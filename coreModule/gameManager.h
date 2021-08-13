#ifndef GENERIC_GAMEMANAGER_H
#define GENERIC_GAMEMANAGER_H

#include "cocos2d.h"
#include "generic/coreModule/scenes/layersEnum.h"
#include <functional>
#include <string>

#define GET_GAME_MANAGER() generic::coreModule::gameManager::getInstance()

namespace generic {

    namespace coreModule {
        class mainScene;
        class windowSystem;
        class windowBase;

        class gameManager {
        public:
            gameManager();
            ~gameManager();
            static gameManager& getInstance();
            void cleanup();

            void run(const std::string& state = "mainMenuScene");
            void changeState(const std::string& state);
            const std::string& getCurrentState() {
                return currentState;
            }
            mainScene* getMainScene() {
                return mainSceneIns;
            }
            windowSystem* getWindowSystem();
            void registerWindow(const std::string&, const std::function<windowBase*()>&);
            windowBase* requestWindow(const std::string&, bool force = false);

        private:
            std::string currentState;
            mainScene* mainSceneIns = nullptr;
        };
    }// namespace coreModule
}// namespace generic


#endif// GENERIC_GAMEMANAGER_H
