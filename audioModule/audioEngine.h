#ifndef MERCENARY_BATTLES_AUDIOENGINE_H
#define MERCENARY_BATTLES_AUDIOENGINE_H

#include <functional>
#include <map>
#include <string>
#include <tuple>
#include "cocos/audio/include/AudioEngine.h"

#define GET_AUDIO_ENGINE() common::audioModule::audioEngine::getInstance()
#define AUDIO_ENGINE_SOUND_DIR "sounds/"
#define AUDIO_ENGINE_SOUND_FILE "sounds.json"
#define AUDIO_ENGINE_INVALID_TAG -1

namespace common::audioModule {

    class audioEngine {
      public:
        audioEngine();
        ~audioEngine();
        static audioEngine& getInstance();
        void cleanup();

        void play(const std::string& filePath, bool loop = false, float volume = 1.0f, const cocos2d::AudioProfile *profile = nullptr);
        void pause(const std::string& name);
        void pauseAll();
        void resume(const std::string& name);
        void resumeAll();
        void stop(const std::string& name);
        void stopAll();
        void preload(const std::string& name, const std::function<void(bool isSuccess)>& clb = nullptr);
        void unload(const std::string& name);

        std::map<std::string, std::pair<std::string, unsigned int>> getAllSounds() { return sounds; }

      private:
        std::map<std::string, std::pair<std::string, unsigned int>> sounds;// key, filePath, id
    };
}// namespace common::audioModule


#endif// MERCENARY_BATTLES_AUDIOENGINE_H
