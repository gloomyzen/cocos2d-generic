#ifndef COMMON_SCENESFACTORYINSTANCE_H
#define COMMON_SCENESFACTORYINSTANCE_H

#include "cocos2d.h"
#include "coreModule/enums/layersEnum.h"
#include "coreModule/enums/statesEnums.h"
#include <map>
#include <functional>
#include <string>

#define GET_SCENES_FACTORY() common::coreModule::scenesFactoryInstance::getInstance()

namespace common {

	namespace coreModule {
		using namespace cocos2d;

		class scenesFactoryInstance {
		public:
			scenesFactoryInstance();
			~scenesFactoryInstance();
			static scenesFactoryInstance &getInstance();
			Layer* getStateRoot(eGameStates);
			void registerState(eGameStates, std::function<Layer*(Layer*)>);
		private:
			std::map<eGameStates, std::function<Layer*(Layer*)>> states{};
		};
	}
}//common::coreModule


#endif //COMMON_SCENESFACTORYINSTANCE_H
