#ifndef COMMON_SCENESFACTORYINSTANCE_H
#define COMMON_SCENESFACTORYINSTANCE_H

#include "cocos2d.h"
#include "common/coreModule/enums/layersEnum.h"
#include <functional>
#include <map>
#include <string>

#define GET_SCENES_FACTORY() common::coreModule::scenesFactoryInstance::getInstance()

namespace common {

    namespace coreModule {
        using namespace cocos2d;

        class scenesFactoryInstance {
          public:
            scenesFactoryInstance();
            ~scenesFactoryInstance();
            static scenesFactoryInstance& getInstance();
            Layer* getStateRoot(const std::string&);
            bool isStateRegistered(const std::string&);
            bool registerState(const std::string&, std::function<Layer*()>);

          private:
            std::map<std::string, bool> registeredStatesMap;
            std::map<std::string, std::function<Layer*()>> states;
        };
    }// namespace coreModule
}// namespace common


#endif// COMMON_SCENESFACTORYINSTANCE_H
