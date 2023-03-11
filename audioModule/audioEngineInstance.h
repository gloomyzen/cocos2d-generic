#pragma once
#ifndef GENERIC_AUDIOENGINEINSTANCE_H
#define GENERIC_AUDIOENGINEINSTANCE_H

#include "audio/AudioEngine.h"
#include <functional>
#include <map>
#include <string>
#include <tuple>

#define GET_AUDIO_ENGINE() generic::audioModule::audioEngineInstance::getInstance()
#define AUDIO_ENGINE_SOUND_DIR "sounds/"
#define AUDIO_ENGINE_SOUND_FILE "sounds.json"
#define AUDIO_ENGINE_INVALID_TAG -1

namespace generic::audioModule {

    class audioEngineInstance {
    public:
        ~audioEngineInstance();
        static audioEngineInstance& getInstance();
        static void cleanup();

        void play(const std::string& filePath, bool loop = false, float volume = 1.0f, const ax::AudioProfile* profile = nullptr);
        void playOnce(const std::string& filePath, bool loop = false, float volume = 1.0f, const ax::AudioProfile* profile = nullptr);
        void pause(const std::string& name);
        void pauseAll();
        void resume(const std::string& name);
        void resumeAll();
        void stop(const std::string& name);
        void stopAll();
        void preload(const std::string& name, const std::function<void(bool isSuccess)>& clb = nullptr);
        void unload(const std::string& name);

        std::map<std::string, std::pair<std::string, unsigned int>> getAllSounds() {
            return sounds;
        }

        void onSceneClosing(const std::string& sceneName);
        void onSceneLoading(const std::string& sceneName);

    private:
        audioEngineInstance();
        audioEngineInstance(const audioEngineInstance&) = default;
        audioEngineInstance& operator=(const audioEngineInstance&) = default;
        static void create();
        static void onDeadReference();

        static audioEngineInstance* pInstance;
        static bool destroyed;

        std::map<std::string, std::pair<std::string, unsigned int>> sounds;// key, filePath, id
        static std::vector<std::string> reservedPrefix;
    };
}// namespace generic::audioModule


#endif// GENERIC_AUDIOENGINEINSTANCE_H
