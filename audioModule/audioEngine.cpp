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
    const std::string& regionStr = cocos2d::FileUtils::getInstance()->getStringFromFile(AUDIO_ENGINE_SOUND_DIR + AUDIO_ENGINE_SOUND_FILE);
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
                effects.insert({value->name.GetString(), { value->value.GetString(), AUDIO_ENGINE_INVALID_TAG } });
                preloadEffect(value->value.GetString());
            }
        }
    }
    auto musicIt = data.FindMember("musics");
    if (musicIt != data.MemberEnd() && musicIt->value.IsObject()) {
        for (auto value = musicIt->value.MemberBegin(); value != musicIt->value.MemberEnd(); ++value) {
            if (value->name.IsString() && value->value.IsString()) {
                musics.insert({value->name.GetString(), { value->value.GetString(), AUDIO_ENGINE_INVALID_TAG } });
                preloadBackgroundMusic(value->value.GetString());
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

