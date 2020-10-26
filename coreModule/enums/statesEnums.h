#ifndef COMMON_STATESENUMS_H
#define COMMON_STATESENUMS_H

#include <map>
#include <string>


namespace common {

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
}//common::coreModule

#endif //COMMON_STATESENUMS_H
