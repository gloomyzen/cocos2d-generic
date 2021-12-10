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

bool scenesFactoryInstance::isSceneRegistered(const std::string& stateName) {
    return scenesMap.count(stateName);
}

bool scenesFactoryInstance::registerScene(const std::string& stateName, const std::function<sceneInterface*()>& clb) {
    if (!isSceneRegistered(stateName)) {
        scenesMap[stateName] = clb;
        return true;
    }
    return false;
}

bool scenesFactoryInstance::runScene(const std::string& stateName) {
    if (isSceneRegistered(stateName)) {
        auto scene = scenesMap[stateName]();
        if (!scene)
            return false;
        if (!currentScene) {
            Director::getInstance()->runWithScene(scene);
        } else {
            currentScene->onSceneClosing();
            Director::getInstance()->replaceScene(TransitionSlideInT::create(1, scene));
        }
        if (scene->isPhysics()) {
            scene->initWithPhysics();
            scene->getPhysicsWorld()->setGravity(scene->getGravity());
        }
        scene->onSceneLoading();
        initTaskLoading(scene);
        currentScene = scene;
        scene->getDefaultCamera()->setName("CameraNode");
        return true;
    }
    return false;
}

void scenesFactoryInstance::cleanup() {
    if (currentFactoryInstance) {
        currentFactoryInstance->scenesMap.clear();
        delete currentFactoryInstance->currentScene;
        currentFactoryInstance->currentScene = nullptr;
        delete currentFactoryInstance;
    }
    currentFactoryInstance = nullptr;
}

void scenesFactoryInstance::initTaskLoading(cocos2d::Node* node) {
    if (auto item = dynamic_cast<taskHolder*>(node)) {
        auto atp = cocos2d::AsyncTaskPool::getInstance();
        atp->enqueue(cocos2d::AsyncTaskPool::TaskType::TASK_OTHER, [](void*) {}, nullptr, [item]() {
            item->executeTasks();
        });
    }
}
