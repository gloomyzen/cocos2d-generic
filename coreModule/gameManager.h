#ifndef COMMON_GAMEMANAGER_H
#define COMMON_GAMEMANAGER_H

#include "cocos2d.h"
#include "enums/layersEnum.h"
#include "enums/statesEnums.h"

#define GET_GAME_MANAGER() common::coreModule::gameManager::getInstance()

namespace common {

	namespace coreModule {
		class mainScene;

		class gameManager {
		public:
			gameManager();
			~gameManager();
			static gameManager &getInstance();

			void run(eGameStates state = eGameStates::MAIN_MENU);
			void changeState(eGameStates state);
			eGameStates getCurrentState() { return currentState; }

		private:
			eGameStates currentState;
			mainScene* mainSceneIns = nullptr;
		};
	}
}//common::coreModule


#endif //COMMON_GAMEMANAGER_H
