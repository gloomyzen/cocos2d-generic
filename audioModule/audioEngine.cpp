#include "audioEngine.h"
#include "editor-support/cocostudio/SimpleAudioEngine.h"
#include "json/document.h"
#include "json/ostreamwrapper.h"
#include "cocos2d.h"
#include "common/debugModule/logManager.h"

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
    auto effectIt = data.FindMember("effects");
    if (effectIt != data.MemberEnd() && effectIt->value.IsObject()) {
        for (auto value = effectIt->value.MemberBegin(); value != effectIt->value.MemberEnd(); ++value) {
            if (value->name.IsString() && value->value.IsString()) {
                auto path = std::string(AUDIO_ENGINE_SOUND_DIR) + value->value.GetString();
                effects.insert({value->name.GetString(), { path.c_str(), AUDIO_ENGINE_INVALID_TAG } });
                preloadEffect(path.c_str());
            }
        }
    }
    auto musicIt = data.FindMember("musics");
    if (musicIt != data.MemberEnd() && musicIt->value.IsObject()) {
        for (auto value = musicIt->value.MemberBegin(); value != musicIt->value.MemberEnd(); ++value) {
            if (value->name.IsString() && value->value.IsString()) {
                auto path = std::string(AUDIO_ENGINE_SOUND_DIR) + value->value.GetString();
                musics.insert({value->name.GetString(), { path.c_str(), AUDIO_ENGINE_INVALID_TAG } });
                preloadBackgroundMusic(path.c_str());
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
    musics.clear();
    effects.clear();
}

void audioEngine::playEffect(const std::string& name, bool loop, float pitch, float pan, float gain) {
    auto item = effects.find(name);
    if (item != effects.end()) {
        auto id = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(item->second.first.c_str(), loop, pitch, pan, gain);
        if (id != AUDIO_ENGINE_INVALID_TAG) {
            effects[name].second = id;
        }
    }
}

void audioEngine::pauseEffect(const std::string& name) {
    auto item = effects.find(name);
    if (item != effects.end()) {
        if (item->second.second != AUDIO_ENGINE_INVALID_TAG) {
            CocosDenshion::SimpleAudioEngine::getInstance()->pauseEffect(item->second.second);
        }
    }
}

void audioEngine::pauseAllEffects() {
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
}

void audioEngine::resumeEffect(const std::string& name) {
    auto item = effects.find(name);
    if (item != effects.end()) {
        if (item->second.second != AUDIO_ENGINE_INVALID_TAG) {
            CocosDenshion::SimpleAudioEngine::getInstance()->resumeEffect(item->second.second);
        }
    }
}

void audioEngine::resumeAllEffects() {
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
}

void audioEngine::stopEffect(const std::string& name) {
    auto item = effects.find(name);
    if (item != effects.end()) {
        if (item->second.second != AUDIO_ENGINE_INVALID_TAG) {
            CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(item->second.second);
        }
        effects[name].second = AUDIO_ENGINE_INVALID_TAG;
    }
}

void audioEngine::stopAllEffects() {
    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
}

void audioEngine::preloadEffect(const std::string& name) {
    auto item = effects.find(name);
    if (item != effects.end()) {
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(item->second.first.c_str());
    }
}

void audioEngine::unloadEffect(const std::string& name) {
    auto item = effects.find(name);
    if (item != effects.end()) {
        CocosDenshion::SimpleAudioEngine::getInstance()->unloadEffect(item->second.first.c_str());
        effects.erase(item);
    }
}
