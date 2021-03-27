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

        unsigned int playEffect(const std::string& fileName,
                                bool loop = false,
                                float pitch = 1.0f,
                                float pan = 0.0f,
                                float gain = 1.0f);
        void pauseEffect(unsigned int soundId);
        void pauseAllEffects();
        void resumeEffect(unsigned int soundId);
        void resumeAllEffects();
        void stopEffect(unsigned int soundId);
        void stopAllEffects();
        void preloadEffect(const std::string& filePath);
        void unloadEffect(const char* filePath);

        void preloadBackgroundMusic(const char* filePath);
        void playBackgroundMusic(const char* filePath, bool loop = false);
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
