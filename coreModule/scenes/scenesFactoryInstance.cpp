#include "scenesFactoryInstance.h"
#include "sceneInterface.h"
#include <map>

using namespace generic;
using namespace generic::coreModule;
using namespace cocos2d;

scenesFactoryInstance* currentFactoryInstance = nullptr;

scenesFactoryInstance::scenesFactoryInstance() = default;

scenesFactoryInstance::~scenesFactoryInstance() = default;

scenesFactoryInstance& scenesFactoryInstance::getInstance() {
    if (currentFactoryInstance == nullptr) {
        currentFactoryInstance = new scenesFactoryInstance();
    }
    return *currentFactoryInstance;
}

bool scenesFactoryInstance::isStateRegistered(const std::string& stateName) {
    return states.count(stateName);
}

bool scenesFactoryInstance::registerState(const std::string& stateName, const std::function<sceneInterface*()>& clb) {
    if (!isStateRegistered(stateName)) {
        states[stateName] = clb;
        return true;
    }
    return false;
}

bool scenesFactoryInstance::runState(const std::string& stateName) {
    if (!isStateRegistered(stateName)) {
        auto scene = states[stateName]();
        if (!scene)
            return false;
        if (!currentScene) {
            Director::getInstance()->runWithScene(scene);
        } else {
            currentScene->onSceneClosing();
            Director::getInstance()->replaceScene(TransitionSlideInT::create(1, scene));
        }
        scene->onSceneLoading();
        currentScene = scene;
        scene->getDefaultCamera()->setName("CameraNode");
        return true;
    }
    return false;
}

void scenesFactoryInstance::cleanup() {
    if (currentFactoryInstance) {
        currentFactoryInstance->states.clear();
        delete currentFactoryInstance->currentScene;
        currentFactoryInstance->currentScene = nullptr;
        delete currentFactoryInstance;
    }
    currentFactoryInstance = nullptr;
}
