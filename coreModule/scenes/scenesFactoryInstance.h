#ifndef GENERIC_SCENESFACTORYINSTANCE_H
#define GENERIC_SCENESFACTORYINSTANCE_H

#include "cocos2d.h"
#include "generic/coreModule/scenes/layersEnum.h"
#include <functional>
#include <map>
#include <string>

#define GET_SCENES_FACTORY() generic::coreModule::scenesFactoryInstance::getInstance()

namespace generic::coreModule {

    class sceneInterface;

    class scenesFactoryInstance {
    public:
        scenesFactoryInstance();
        ~scenesFactoryInstance();
        static scenesFactoryInstance& getInstance();
        static void cleanup();
        bool isStateRegistered(const std::string& stateName);
        bool registerState(const std::string& stateName, const std::function<sceneInterface*()>& clb);
        bool runState(const std::string& stateName);

    private:
        std::map<std::string, std::function<sceneInterface*()>> states;
        sceneInterface* currentScene = nullptr;
    };
}// namespace generic::coreModule


#endif// GENERIC_SCENESFACTORYINSTANCE_H
