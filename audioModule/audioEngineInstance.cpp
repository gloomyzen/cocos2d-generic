#include "audioEngineInstance.h"

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "generic/debugModule/logManager.h"
#include "generic/utilityModule/jsonHelper.h"
#include <utility>

using namespace generic::audioModule;

audioEngineInstance* audioEngineInstance::pInstance = nullptr;
bool audioEngineInstance::destroyed = false;

audioEngineInstance::audioEngineInstance() {
    // load json
    const std::string& regionStr =
      cocos2d::FileUtils::getInstance()->getStringFromFile(std::string(AUDIO_ENGINE_SOUND_DIR) + std::string(AUDIO_ENGINE_SOUND_FILE));
    rapidjson::Document data;
    data.Parse<0>(regionStr.c_str());

    if (data.HasParseError() || data.IsNull() || !data.IsObject()) {
        LOG_ERROR("Json parse error");
        return;
    }
    auto effectIt = data.FindMember("sounds");
    if (effectIt != data.MemberEnd() && effectIt->value.IsObject()) {
        for (auto value = effectIt->value.MemberBegin(); value != effectIt->value.MemberEnd(); ++value) {
            if (value->name.IsString() && value->value.IsString()) {
                auto path = std::string(AUDIO_ENGINE_SOUND_DIR) + value->value.GetString();
                sounds.insert({ value->name.GetString(), { path, AUDIO_ENGINE_INVALID_TAG } });
                preload(path);
            }
        }
    }
}

void audioEngineInstance::create() {
    static audioEngineInstance instance;
    pInstance = &instance;
}

void audioEngineInstance::onDeadReference() {
    CCASSERT(false, "Founded dead reference!");
}

audioEngineInstance::~audioEngineInstance() {
    sounds.clear();
}

audioEngineInstance& audioEngineInstance::getInstance() {
    if (!pInstance) {
        if (destroyed) {
            onDeadReference();
        } else {
            create();
        }
    }
    return *pInstance;
}

void audioEngineInstance::cleanup() {
    destroyed = true;
    pInstance = nullptr;
}

void audioEngineInstance::play(const std::string& name, bool loop, float volume, const cocos2d::AudioProfile* profile) {
    if (sounds.count(name)) {
        auto id = cocos2d::AudioEngine::play2d(sounds[name].first, loop, volume, profile);
        if (id != AUDIO_ENGINE_INVALID_TAG) {
            sounds[name].second = id;
        }
    }
}

void audioEngineInstance::pause(const std::string& name) {
    if (sounds.count(name)) {
        if (sounds[name].second != AUDIO_ENGINE_INVALID_TAG) {
            cocos2d::AudioEngine::pause(sounds[name].second);
        }
    }
}

void audioEngineInstance::pauseAll() {
    for (const auto& item : sounds) {
        cocos2d::AudioEngine::pause(item.second.second);
    }
}

void audioEngineInstance::resume(const std::string& name) {
    if (sounds.count(name)) {
        if (sounds[name].second != AUDIO_ENGINE_INVALID_TAG) {
            cocos2d::AudioEngine::resume(sounds[name].second);
        }
    }
}

void audioEngineInstance::resumeAll() {
    for (const auto& item : sounds) {
        cocos2d::AudioEngine::resume(item.second.second);
    }
}

void audioEngineInstance::stop(const std::string& name) {
    if (sounds.count(name)) {
        if (sounds[name].second != AUDIO_ENGINE_INVALID_TAG) {
            cocos2d::AudioEngine::stop(sounds[name].second);
        }
        sounds[name].second = AUDIO_ENGINE_INVALID_TAG;
    }
}

void audioEngineInstance::stopAll() {
    for (const auto& item : sounds) {
        cocos2d::AudioEngine::stop(item.second.second);
    }
}

void audioEngineInstance::preload(const std::string& name, const std::function<void(bool)>& clb) {
    if (sounds.count(name)) {
        cocos2d::AudioEngine::preload(sounds[name].first, clb);
    }
}

void audioEngineInstance::unload(const std::string& name) {
    if (sounds.count(name)) {
        cocos2d::AudioEngine::uncache(sounds[name].first);
        auto it = sounds.find(name);
        sounds.erase(it);
    }
}
