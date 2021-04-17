#include "scenesFactoryInstance.h"
#include "common/debugModule/logManager.h"
#include "common/coreModule/gameManager.h"
#include "common/coreModule/scenes/mainScene.h"
#include "common/utilityModule/stringUtility.h"
#include <map>

using namespace common;
using namespace common::coreModule;
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

Layer* scenesFactoryInstance::getStateRoot(const std::string& state) {
    if (isStateRegistered(state)) {
		return states[state]();
    }
    LOG_ERROR(
        STRING_FORMAT("scenesFactoryInstance::getStateRoot: Current state '%s' is not registered! Return simple layer.",
                      state.c_str()));
    return Layer::create();
}

bool scenesFactoryInstance::registerState(const std::string& state, std::function<Layer*()> clb) {
    if (isStateRegistered(state))
        return false;

    states[state] = std::move(clb);
    registeredStatesMap[state] = true;
    return true;
}

bool scenesFactoryInstance::isStateRegistered(const std::string& needle) {
    return registeredStatesMap.count(needle) != 0;
}
