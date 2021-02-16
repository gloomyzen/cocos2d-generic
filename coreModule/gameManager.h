#ifndef COMMON_GAMEMANAGER_H
#define COMMON_GAMEMANAGER_H

#include "cocos2d.h"
#include "enums/layersEnum.h"
#include <string>
#include <functional>

#define GET_GAME_MANAGER() common::coreModule::gameManager::getInstance()

namespace common {

	namespace coreModule {
		class mainScene;
		class windowSystem;
		class windowBase;

		class gameManager {
		public:
			gameManager();
			~gameManager();
			static gameManager &getInstance();

			void run(const std::string& state = "mainMenuScene");
			void changeState(const std::string& state);
			const std::string& getCurrentState() { return currentState; }
			mainScene* getMainScene() { return mainSceneIns; }
			cocos2d::EventListenerTouchOneByOne* getListener();
			windowSystem* getWindowSystem();
			void registerWindow(const std::string&, const std::function<windowBase*()>&);
			bool requestWindow(const std::string&);

		private:
			std::string currentState;
			mainScene* mainSceneIns = nullptr;
		};
	}
}//common::coreModule


#endif //COMMON_GAMEMANAGER_H
