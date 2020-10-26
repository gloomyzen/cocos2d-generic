#include "scenesFactoryInstance.h"
#include "common/debugModule/logManager.h"
#include "common/coreModule/enums/statesEnums.h"
#include <map>

using namespace common;
using namespace common::coreModule;
using namespace cocos2d;

scenesFactoryInstance *currentFactoryInstance = nullptr;

scenesFactoryInstance::scenesFactoryInstance() = default;

scenesFactoryInstance::~scenesFactoryInstance() = default;

scenesFactoryInstance &scenesFactoryInstance::getInstance() {
	if (currentFactoryInstance == nullptr) {
		currentFactoryInstance = new scenesFactoryInstance();
	}
	return *currentFactoryInstance;
}

Layer *scenesFactoryInstance::getStateRoot(eGameStates state) {
	if (states.count(state)) {
		auto layer = Layer::create();
		layer->setName(mapSceneNames[state]);
		return states[state](layer);
	}
	LOG_ERROR("scenesFactoryInstance::getStateRoot: Current state " + std::to_string(state) + " is not registered! Return simple layer.");
	return Layer::create();
}

void scenesFactoryInstance::registerState(eGameStates state, std::function<Layer *(Layer*)> clb) {
	states[state] = std::move(clb);
}
