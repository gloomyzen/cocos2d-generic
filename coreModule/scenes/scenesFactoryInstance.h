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
        bool isSceneRegistered(const std::string& stateName);
        bool registerScene(const std::string& stateName, const std::function<sceneInterface*()>& clb);
        bool runScene(const std::string& stateName);

    private:
        void initTaskLoading(cocos2d::Node* node);

        std::map<std::string, std::function<sceneInterface*()>> scenesMap;
        sceneInterface* currentScene = nullptr;
    };
}// namespace generic::coreModule


#endif// GENERIC_SCENESFACTORYINSTANCE_H
