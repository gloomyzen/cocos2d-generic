#ifndef GENERIC_AUDIOENGINE_H
#define GENERIC_AUDIOENGINE_H

#include "cocos/audio/include/AudioEngine.h"
#include <functional>
#include <map>
#include <string>
#include <tuple>

#define GET_AUDIO_ENGINE() generic::audioModule::audioEngine::getInstance()
#define AUDIO_ENGINE_SOUND_DIR "sounds/"
#define AUDIO_ENGINE_SOUND_FILE "sounds.json"
#define AUDIO_ENGINE_INVALID_TAG -1

namespace generic::audioModule {

    class audioEngine {
    public:
        audioEngine();
        ~audioEngine();
        static audioEngine& getInstance();
        void cleanup();

        void play(const std::string& filePath, bool loop = false, float volume = 1.0f, const cocos2d::AudioProfile* profile = nullptr);
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

    private:
        std::map<std::string, std::pair<std::string, unsigned int>> sounds;// key, filePath, id
    };
}// namespace generic::audioModule


#endif// GENERIC_AUDIOENGINE_H
