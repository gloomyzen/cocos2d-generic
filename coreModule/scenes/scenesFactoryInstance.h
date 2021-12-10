#ifndef GENERIC_SCENESFACTORYINSTANCE_H
#define GENERIC_SCENESFACTORYINSTANCE_H

#include "cocos2d.h"
#include "generic/coreModule/scenes/layersEnum.h"
#include <functional>
#include <map>
#include <string>

#define GET_SCENES_FACTORY() generic::coreModule::scenesFactoryInstance::getInstance()

namespace generic::coreModule {

    class scenesFactoryInstance {
    public:
        scenesFactoryInstance();
        ~scenesFactoryInstance();
        static scenesFactoryInstance& getInstance();
        cocos2d::Node* getStateRoot(const std::string&);
        bool isStateRegistered(const std::string&);
        bool registerState(const std::string&, std::function<cocos2d::Node*()>);

    private:
        std::map<std::string, bool> registeredStatesMap;
        std::map<std::string, std::function<cocos2d::Node*()>> states;
    };
}// namespace generic::coreModule


#endif// GENERIC_SCENESFACTORYINSTANCE_H
