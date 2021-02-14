#ifndef COMMON_MAINSCENE_H
#define COMMON_MAINSCENE_H

#include "cocos2d.h"
#include "common/coreModule/nodes/nodeProperties.h"
#include "common/coreModule/scenes/windows/windowSystem.h"
#include <vector>
#include <string>

namespace common {
	namespace coreModule {

	class mainScene : public cocos2d::Scene {
		public:
			static cocos2d::Scene *createScene();

			virtual bool init();

			void setRoom(const std::string&);


			void menuCloseCallback(cocos2d::Ref *pSender);

			CREATE_FUNC(mainScene);

			windowSystem* getWindowNode() { return windowViewer; }

		private:
			std::vector<cocos2d::Node*> nodes{};
			windowSystem* windowViewer = nullptr;
		};
	}
}//common::coreModule


#endif //COMMON_MAINSCENE_H
