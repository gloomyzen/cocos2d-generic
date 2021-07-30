#ifndef GENERIC_SCENESFACTORYINSTANCE_H
#define GENERIC_SCENESFACTORYINSTANCE_H

#include "cocos2d.h"
#include "generic/coreModule/scenes/layersEnum.h"
#include <functional>
#include <map>
#include <string>

#define GET_SCENES_FACTORY() generic::coreModule::scenesFactoryInstance::getInstance()

namespace generic {

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
}// namespace generic


#endif// GENERIC_SCENESFACTORYINSTANCE_H
