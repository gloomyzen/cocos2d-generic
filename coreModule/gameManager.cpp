#include "gameManager.h"
#include "generic/coreModule/nodes/types/eventNode.h"
#include "generic/coreModule/scenes/mainScene.h"
#include "generic/coreModule/scenes/windows/windowBase.h"
#include "generic/coreModule/scenes/windows/windowSystem.h"
#include "generic/debugModule/logManager.h"

using namespace generic;
using namespace generic::coreModule;
using namespace cocos2d;

gameManager* currentGameManager = nullptr;

gameManager::gameManager() {
    currentState = "loadingScreenScene";
    mainSceneIns = dynamic_cast<mainScene*>(mainScene::createScene());
    Director::getInstance()->runWithScene(mainSceneIns);
    mainSceneIns->getDefaultCamera()->setName("CameraNode");
}

gameManager::~gameManager() {}

gameManager& gameManager::getInstance() {
    if (currentGameManager == nullptr) {
        currentGameManager = new gameManager();
    }
    return *currentGameManager;
}

void gameManager::run(const std::string& state) {
    /// Starting from state
    changeState(state);
}

void gameManager::changeState(const std::string& state) {
    if (mainSceneIns == nullptr) {
        LOG_ERROR("Instance mainSceneIns is null!");
        return;
    }
    currentState = state;
    mainSceneIns->setRoom(currentState);
}

windowSystem* gameManager::getWindowSystem() {
    if (!mainSceneIns)
        return nullptr;
    return mainSceneIns->getWindowNode();
}

void gameManager::registerWindow(const std::string& name, const std::function<windowBase*()>& clb) {
    getWindowSystem()->registerWindow(name, clb);
}

windowBase* gameManager::requestWindow(const std::string& name, bool force) {
    return getWindowSystem()->requestWindow(name, force);
}

void gameManager::cleanup() {
    delete currentGameManager;
    currentGameManager = nullptr;
}
