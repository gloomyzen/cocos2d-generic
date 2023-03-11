#include "scenesFactoryInstance.h"
#include "sceneInterface.h"
#include "generic/utilityModule/stringUtility.h"
#include <map>

using namespace generic;
using namespace generic::coreModule;
using namespace ax;

scenesFactoryInstance* scenesFactoryInstance::_pInstance = nullptr;
bool scenesFactoryInstance::_destroyed = false;

scenesFactoryInstance& scenesFactoryInstance::getInstance() {
    if (!_pInstance) {
        if (_destroyed) {
            onDeadReference();
        } else {
            create();
        }
    }
    return *_pInstance;
}

void scenesFactoryInstance::cleanup() {
    _destroyed = true;
    if (_pInstance) {
        AX_SAFE_RETAIN(_pInstance->_currentScene);
    }
    _pInstance = nullptr;
}

void scenesFactoryInstance::create() {
    static scenesFactoryInstance instance;
    _pInstance = &instance;
}

void scenesFactoryInstance::onDeadReference() {
    AXASSERT(false, "Founded dead reference!");
}

bool scenesFactoryInstance::isSceneRegistered(const std::string& sceneName) {
    return _scenesMap.count(sceneName);
}

bool scenesFactoryInstance::registerScene(const std::string& sceneName, const std::function<sceneInterface*()>& clb) {
    if (!isSceneRegistered(sceneName)) {
        _scenesMap[sceneName] = clb;
        return true;
    }
    return false;
}

bool scenesFactoryInstance::runSceneWithParameters(const std::string& sceneName, const ValueMap& values) {
    if (isSceneRegistered(sceneName)) {
        auto scene = _scenesMap[sceneName]();
        if (!scene)
            return false;
        scene->setValues(values);
        bool isFirstStart = _currentScene == nullptr;
        if (scene->isPhysics()) {
            scene->initWithPhysics();
            scene->getPhysicsWorld()->setGravity(scene->getGravity());
        } else {
            scene->init();
        }
        scene->updateLayers(_currentScene);
        if (_currentScene) {
            _currentScene->onSceneClosing();
        }
        _currentScene = scene;
        if (isFirstStart) {
            Director::getInstance()->runWithScene(scene);
        } else {
            if (scene->getFadeTransition() == 0.f) {
                Director::getInstance()->replaceScene(scene);
            } else {
                Director::getInstance()->replaceScene(TransitionProgressHorizontal::create(scene->getFadeTransition(), scene));
            }
        }
        scene->onSceneLoading();
        initTaskLoading(scene);
        if (auto camera = scene->getDefaultCamera()) {
            camera->setName("CameraNode");
        } else {
            AX_ASSERT(0 && "Can't find the camera, probably scene wasn't inited.");
            return false;
        }
        return true;
    }
    return false;
}

bool scenesFactoryInstance::runScene(const std::string& sceneName) {
    return runSceneWithParameters(sceneName, {});
}

void scenesFactoryInstance::initTaskLoading(ax::Node* node) {
//    if (auto item = dynamic_cast<taskHolder*>(node)) {
//        auto atp = ax::AsyncTaskPool::getInstance();
//        atp->enqueue(ax::AsyncTaskPool::TaskType::TASK_OTHER, [](void*) {}, nullptr, [item]() {
//            item->executeTasks();
//        });
//    }
}
