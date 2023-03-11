#pragma once
#ifndef GENERIC_SCENESFACTORYINSTANCE_H
#define GENERIC_SCENESFACTORYINSTANCE_H

#include "axmol.h"
#include "generic/coreModule/scenes/layersEnum.h"
#include <functional>
#include <map>
#include <string>

#define GET_SCENES_FACTORY() generic::coreModule::scenesFactoryInstance::getInstance()
#define GET_CURRENT_SCENE() generic::coreModule::scenesFactoryInstance::getInstance().getCurrentScene()

namespace generic::coreModule {

    class sceneInterface;

    class scenesFactoryInstance {
    public:
        static scenesFactoryInstance& getInstance();
        static void cleanup();
        bool isSceneRegistered(const std::string& sceneName);
        bool registerScene(const std::string& sceneName, const std::function<sceneInterface*()>& clb);
        bool runScene(const std::string& sceneName);
        sceneInterface* getCurrentScene() { return _currentScene; }
        bool runSceneWithParameters(const std::string& sceneName, const ax::ValueMap& values);

    private:
        scenesFactoryInstance() = default;
        ~scenesFactoryInstance() = default;
        scenesFactoryInstance(const scenesFactoryInstance&) = default;
        scenesFactoryInstance& operator=(const scenesFactoryInstance&) = default;
        static void create();
        static void onDeadReference();

        static scenesFactoryInstance* _pInstance;
        static bool _destroyed;

        void initTaskLoading(ax::Node* node);

        std::map<std::string, std::function<sceneInterface*()>> _scenesMap;
        sceneInterface* _currentScene = nullptr;
    };
}// namespace generic::coreModule


#endif// GENERIC_SCENESFACTORYINSTANCE_H
