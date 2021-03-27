#ifndef MERCENARY_BATTLES_AUDIOENGINE_H
#define MERCENARY_BATTLES_AUDIOENGINE_H

#include <functional>
#include <map>
#include <string>
#include <tuple>

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

        void playEffect(const std::string& name,
                                bool loop = false,
                                float pitch = 1.0f,
                                float pan = 0.0f,
                                float gain = 1.0f);
        void pauseEffect(const std::string& name);
        void pauseAllEffects();
        void resumeEffect(const std::string& name);
        void resumeAllEffects();
        void stopEffect(const std::string& name);
        void stopAllEffects();
        void preloadEffect(const std::string& name);
        void unloadEffect(const std::string& name);

        void preloadBackgroundMusic(const char* name);
        void playBackgroundMusic(const char* name, bool loop = false);
        void stopBackgroundMusic(bool releaseData = false);
        void pauseBackgroundMusic();
        void resumeBackgroundMusic();
        void rewindBackgroundMusic();
        bool willPlayBackgroundMusic();

      private:
        std::map<std::string, std::pair<std::string, unsigned int>> musics;// key, file, id
        std::map<std::string, std::pair<std::string, unsigned int>> effects;// key, file, id
    };
}// namespace common::audioModule


#endif// MERCENARY_BATTLES_AUDIOENGINE_H
