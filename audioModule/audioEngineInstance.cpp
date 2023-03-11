#include "audioEngineInstance.h"

#include "axmol.h"
#include "generic/utilityModule/logManager.h"
#include "generic/utilityModule/jsonHelper.h"
#include <utility>

using namespace generic::audioModule;

audioEngineInstance* audioEngineInstance::pInstance = nullptr;
bool audioEngineInstance::destroyed = false;

std::vector<std::string> audioEngineInstance::reservedPrefix = {"ui.", "music."};

audioEngineInstance::audioEngineInstance() {
    // load json
    const std::string& regionStr =
      ax::FileUtils::getInstance()->getStringFromFile(std::string(AUDIO_ENGINE_SOUND_DIR) + std::string(AUDIO_ENGINE_SOUND_FILE));
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
                std::string key = value->name.GetString();
                auto globalSound = std::find_if(reservedPrefix.begin(), reservedPrefix.end(), [key](const std::string& n){
                    return key.find(n) != std::string::npos;
                });
                if (globalSound != reservedPrefix.end())
                    preload(value->name.GetString());
            }
        }
    }
    //loading fairyGUI sounds if exists
    auto fSoundIt = data.FindMember("fairyGUI");
    if (fSoundIt != data.MemberEnd() && fSoundIt->value.IsArray()) {
        for (auto value = fSoundIt->value.Begin(); value != fSoundIt->value.End(); ++value) {
            if (value->IsString()) {
                preload(value->GetString());
            }
        }
    }
}

void audioEngineInstance::create() {
    static audioEngineInstance instance;
    pInstance = &instance;
}

void audioEngineInstance::onDeadReference() {
    AXASSERT(false, "Founded dead reference!");
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

void audioEngineInstance::play(const std::string& name, bool loop, float volume, const ax::AudioProfile* profile) {
#ifndef DISABLE_AUDIO_ENGINE
    if (sounds.count(name)) {
        auto id = ax::AudioEngine::play2d(sounds[name].first, loop, volume, profile);
        if (id != AUDIO_ENGINE_INVALID_TAG) {
            sounds[name].second = id;
        }
    }
#endif
}

void audioEngineInstance::playOnce(const std::string& name, bool loop, float volume, const ax::AudioProfile* profile) {
#ifndef DISABLE_AUDIO_ENGINE
    if (sounds.count(name) && sounds[name].second == AUDIO_ENGINE_INVALID_TAG) {
        auto id = ax::AudioEngine::play2d(sounds[name].first, loop, volume, profile);
        if (id != AUDIO_ENGINE_INVALID_TAG) {
            sounds[name].second = id;
        }
    }
#endif
}

void audioEngineInstance::pause(const std::string& name) {
#ifndef DISABLE_AUDIO_ENGINE
    if (sounds.count(name)) {
        if (sounds[name].second != AUDIO_ENGINE_INVALID_TAG) {
            ax::AudioEngine::pause(sounds[name].second);
        }
    }
#endif
}

void audioEngineInstance::pauseAll() {
#ifndef DISABLE_AUDIO_ENGINE
    for (const auto& item : sounds) {
        ax::AudioEngine::pause(item.second.second);
    }
#endif
}

void audioEngineInstance::resume(const std::string& name) {
#ifndef DISABLE_AUDIO_ENGINE
    if (sounds.count(name)) {
        if (sounds[name].second != AUDIO_ENGINE_INVALID_TAG) {
            ax::AudioEngine::resume(sounds[name].second);
        }
    }
#endif
}

void audioEngineInstance::resumeAll() {
#ifndef DISABLE_AUDIO_ENGINE
    for (const auto& item : sounds) {
        ax::AudioEngine::resume(item.second.second);
    }
#endif
}

void audioEngineInstance::stop(const std::string& name) {
    if (sounds.count(name)) {
        if (sounds[name].second != AUDIO_ENGINE_INVALID_TAG) {
            ax::AudioEngine::stop(sounds[name].second);
        }
        sounds[name].second = AUDIO_ENGINE_INVALID_TAG;
    }
}

void audioEngineInstance::stopAll() {
#ifndef DISABLE_AUDIO_ENGINE
    for (const auto& item : sounds) {
        ax::AudioEngine::stop(item.second.second);
    }
#endif
}

void audioEngineInstance::preload(const std::string& name, const std::function<void(bool)>& clb) {
#ifndef DISABLE_AUDIO_ENGINE
    if (sounds.count(name)) {
        ax::AudioEngine::preload(sounds[name].first, clb);
    }
#endif
}

void audioEngineInstance::unload(const std::string& name) {
#ifndef DISABLE_AUDIO_ENGINE
    if (sounds.count(name)) {
        ax::AudioEngine::uncache(sounds[name].first);
    }
#endif
}

void audioEngineInstance::onSceneClosing(const std::string& sceneName) {
    for (const auto& [key, item] : sounds) {
        if (key.find(sceneName) != std::string::npos) {
            unload(key);
        }
    }
}

void audioEngineInstance::onSceneLoading(const std::string& sceneName) {
    for (const auto& [key, item] : sounds) {
        if (key.find(sceneName) != std::string::npos) {
            preload(key);
        }
    }
}
