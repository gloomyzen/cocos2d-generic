#ifndef MERCENARY_BATTLES_MAINSCENE_H
#define MERCENARY_BATTLES_MAINSCENE_H

#include "cocos2d.h"
#include "coreModule/nodes/nodeProperties.h"
#include "coreModule/enums/statesEnums.h"
#include <vector>

namespace mb {
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
}//mb::coreModule


#endif //MERCENARY_BATTLES_MAINSCENE_H
