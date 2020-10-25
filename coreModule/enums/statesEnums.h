#ifndef MERCENARY_BATTLES_STATESENUMS_H
#define MERCENARY_BATTLES_STATESENUMS_H

#include <map>
#include <string>


namespace mb {

	namespace coreModule {

		enum eGameStates {
			LOADING_SCREEN = 0,
			MAIN_MENU,
			BATTLE_SCENE
		};

		static std::map<eGameStates, std::string> mapSceneNames = {
				{LOADING_SCREEN, "loadingScreenScene"},
				{MAIN_MENU, "mainMenuScene"},
				{BATTLE_SCENE, "battleScene"},
		};
	}
}//mb::coreModule

#endif //MERCENARY_BATTLES_STATESENUMS_H
