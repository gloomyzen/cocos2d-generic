#ifndef COMMON_MAINSCENE_H
#define COMMON_MAINSCENE_H

#include "cocos2d.h"
#include "common/coreModule/nodes/nodeProperties.h"
#include "common/coreModule/enums/statesEnums.h"
#include <vector>

namespace common {
	namespace coreModule {

	using namespace cocos2d;

		class mainScene : public Scene {
		public:
			static cocos2d::Scene *createScene();

			virtual bool init();

			void setRoom(eGameStates);


			void menuCloseCallback(cocos2d::Ref *pSender);

			CREATE_FUNC(mainScene);

		private:
			std::vector<Node*> nodes{};
			cocos2d::Layer* gameLayer;
		};
	}
}//common::coreModule


#endif //COMMON_MAINSCENE_H
