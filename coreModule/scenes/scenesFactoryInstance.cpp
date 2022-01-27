#include "scenesFactoryInstance.h"
#include "sceneInterface.h"
#include "generic/debugModule/imGuiLayer.h"
#include "generic/utilityModule/stringUtility.h"
#include <map>

using namespace generic;
using namespace generic::coreModule;
using namespace cocos2d;

scenesFactoryInstance* scenesFactoryInstance::pInstance = nullptr;
bool scenesFactoryInstance::destroyed = false;

scenesFactoryInstance& scenesFactoryInstance::getInstance() {
    if (!pInstance) {
        if (destroyed) {
            onDeadReference();
        } else {
            create();
        }
    }
    return *pInstance;
}

void scenesFactoryInstance::cleanup() {
    destroyed = true;
    if (pInstance) {
        pInstance->scenesMap.clear();
        CC_SAFE_RETAIN(pInstance->currentScene);
    }
    delete pInstance;
    pInstance = nullptr;
}

void scenesFactoryInstance::create() {
    static scenesFactoryInstance instance;
    pInstance = &instance;
}

void scenesFactoryInstance::onDeadReference() {
    CCASSERT(false, "Founded dead reference!");
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
            Director::getInstance()->replaceScene(TransitionCrossFade::create(scene->getFadeTransition(), scene));
        }
        if (scene->isPhysics()) {
            scene->initWithPhysics();
            scene->getPhysicsWorld()->setGravity(scene->getGravity());
        }
        scene->updateLayers(currentScene);
        scene->onSceneLoading();
        initTaskLoading(scene);
        currentScene = scene;
        scene->getDefaultCamera()->setName("CameraNode");
        if (scene->isPhysics()) {
#ifdef DEBUG
            auto clb = [this]() {
                if (!currentScene)
                    return;
                auto physicsDebugDraw = currentScene->isPhysicsDebugDraw();
                auto label = STRING_FORMAT("Physics debug: %s", physicsDebugDraw ? "ON" : "OFF");
                if (ImGui::Button(label.c_str())) {
                    physicsDebugDraw = !physicsDebugDraw;
                }
                if (currentScene->isPhysicsDebugDraw() != physicsDebugDraw) {
                    if (physicsDebugDraw)
                        LOG_INFO("Enable physics debug for current scene.");
                    else
                        LOG_INFO("Disable physics debug for current scene.");
                    if (!currentScene->isPhysics()) {
                        LOG_ERROR("Physics in current scene is disabled!");
                        return;
                    }
                    currentScene->setPhysicsDebugDraw(physicsDebugDraw);
                    currentScene->getPhysicsWorld()->setDebugDrawMask(
                      physicsDebugDraw ? cocos2d::PhysicsWorld::DEBUGDRAW_ALL : cocos2d::PhysicsWorld::DEBUGDRAW_NONE);
                }
            };
            currentScene->getImGuiLayer()->addDebugModules({ "Physics debug: OFF", [clb]() {
                                                                clb();
                                                            } });
#endif
        }
        return true;
    }
    return false;
}

void scenesFactoryInstance::initTaskLoading(cocos2d::Node* node) {
    if (auto item = dynamic_cast<taskHolder*>(node)) {
        auto atp = cocos2d::AsyncTaskPool::getInstance();
        atp->enqueue(cocos2d::AsyncTaskPool::TaskType::TASK_OTHER, [](void*) {}, nullptr, [item]() {
            item->executeTasks();
        });
    }
}
