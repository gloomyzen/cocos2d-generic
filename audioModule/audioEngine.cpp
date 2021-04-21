#include "audioEngine.h"

#include <utility>
#include "cocos2d.h"
#include "common/debugModule/logManager.h"
#include "extensions/cocos-ext.h"
#include "extensions/cocostudio/CocoStudio.h"
#include "rapidjson/document.h"

using namespace common::audioModule;

audioEngine* audioEngineInstance = nullptr;

audioEngine::audioEngine() {
    // load json
    const std::string& regionStr = cocos2d::FileUtils::getInstance()->getStringFromFile(std::string(AUDIO_ENGINE_SOUND_DIR) + std::string(AUDIO_ENGINE_SOUND_FILE));
    rapidjson::Document data;
    data.Parse<0>(regionStr.c_str());

    if (data.HasParseError() || data.IsNull() || !data.IsObject()) {
        LOG_ERROR("audioEngine::audioEngine: json parse error");
        return;
    }
    auto effectIt = data.FindMember("sounds");
    if (effectIt != data.MemberEnd() && effectIt->value.IsObject()) {
        for (auto value = effectIt->value.MemberBegin(); value != effectIt->value.MemberEnd(); ++value) {
            if (value->name.IsString() && value->value.IsString()) {
                auto path = std::string(AUDIO_ENGINE_SOUND_DIR) + value->value.GetString();
                sounds.insert({value->name.GetString(), { path, AUDIO_ENGINE_INVALID_TAG } });
                preload(path);
            }
        }
    }
}

audioEngine::~audioEngine() {
    cleanup();
}

audioEngine& audioEngine::getInstance() {
    if (audioEngineInstance == nullptr) {
        audioEngineInstance = new audioEngine();
    }
    return *audioEngineInstance;
}

void audioEngine::cleanup() {
    sounds.clear();
    cocos2d::AudioEngine::end();
}

void audioEngine::play(const std::string& name, bool loop, float volume, const cocos2d::AudioProfile *profile) {
    auto item = sounds.find(name);
    if (item != sounds.end()) {
        auto id = cocos2d::AudioEngine::play2d(item->second.first, loop, volume, profile);
        if (id != AUDIO_ENGINE_INVALID_TAG) {
            sounds[name].second = id;
        }
    }
}

void audioEngine::pause(const std::string& name) {
    auto item = sounds.find(name);
    if (item != sounds.end()) {
        if (item->second.second != AUDIO_ENGINE_INVALID_TAG) {
            cocos2d::AudioEngine::pause(item->second.second);
        }
    }
}

void audioEngine::pauseAll() {
    for (const auto& item : sounds) {
        cocos2d::AudioEngine::pause(item.second.second);
    }
}

void audioEngine::resume(const std::string& name) {
    auto item = sounds.find(name);
    if (item != sounds.end()) {
        if (item->second.second != AUDIO_ENGINE_INVALID_TAG) {
            cocos2d::AudioEngine::resume(item->second.second);
        }
    }
}

void audioEngine::resumeAll() {
    for (const auto& item : sounds) {
        cocos2d::AudioEngine::resume(item.second.second);
    }
}

void audioEngine::stop(const std::string& name) {
    auto item = sounds.find(name);
    if (item != sounds.end()) {
        if (item->second.second != AUDIO_ENGINE_INVALID_TAG) {
            cocos2d::AudioEngine::stop(item->second.second);
        }
        sounds[name].second = AUDIO_ENGINE_INVALID_TAG;
    }
}

void audioEngine::stopAll() {
    for (const auto& item : sounds) {
        cocos2d::AudioEngine::stop(item.second.second);
    }
}

void audioEngine::preload(const std::string& name, const std::function<void(bool)>& clb) {
    auto item = sounds.find(name);
    if (item != sounds.end()) {
        cocos2d::AudioEngine::preload(item->second.first, clb);
    }
}

void audioEngine::unload(const std::string& name) {
    auto item = sounds.find(name);
    if (item != sounds.end()) {
        cocos2d::AudioEngine::uncache(item->second.first);
        sounds.erase(item);
    }
}
