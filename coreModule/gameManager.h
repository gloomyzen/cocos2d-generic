#ifndef MERCENARY_BATTLES_GAMEMANAGER_H
#define MERCENARY_BATTLES_GAMEMANAGER_H

#include "cocos2d.h"
#include "enums/layersEnum.h"
#include "enums/statesEnums.h"

#define GET_GAME_MANAGER() mb::coreModule::gameManager::getInstance()

namespace mb {

	namespace coreModule {
		class mainScene;
		using namespace cocos2d;

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
}//mb::coreModule


#endif //MERCENARY_BATTLES_GAMEMANAGER_H
