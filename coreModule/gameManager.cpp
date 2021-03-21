#include "gameManager.h"
#include "common/coreModule/nodes/widgets/eventNode.h"
#include "common/coreModule/scenes/mainScene.h"
#include "common/coreModule/scenes/windows/windowBase.h"
#include "common/coreModule/scenes/windows/windowSystem.h"
#include "common/debugModule/logManager.h"

using namespace common;
using namespace common::coreModule;

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
        LOG_ERROR("gameManager::changeState Instance mainSceneIns is null!");
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
