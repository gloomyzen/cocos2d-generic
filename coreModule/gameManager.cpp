#include "gameManager.h"
#include "common/coreModule/scenes/mainScene.h"
#include "common/debugModule/logManager.h"
#include "common/coreModule/nodes/widgets/eventNode.h"

using namespace common;
using namespace common::coreModule;

gameManager *currentGameManager = nullptr;

gameManager::gameManager() {
	currentState = "loadingScreenScene";
	mainSceneIns = dynamic_cast<mainScene *>(mainScene::createScene());
};

gameManager::~gameManager() {}

gameManager &gameManager::getInstance() {
	if (currentGameManager == nullptr) {
		currentGameManager = new gameManager();
	}
	return *currentGameManager;
}

void gameManager::run(const std::string& state) {
	/// Starting from state
	changeState(state);
	Director::getInstance()->runWithScene(mainSceneIns);

	//todo remove after testing
//	auto seq = Sequence::create(DelayTime::create(7.f), CallFunc::create([this](){
//		changeState("battleScene");
//	}), nullptr);
//	mainSceneIns->runAction(seq);

//	auto seq = Sequence::create(DelayTime::create(7.f), CallFunc::create([this](){
//		changeState("battleScene");
//	}),DelayTime::create(5.f), CallFunc::create([this](){
//		changeState("mainMenuScene");
//	}),DelayTime::create(5.f), CallFunc::create([this](){
//		changeState("battleScene");
//	}),DelayTime::create(5.f), CallFunc::create([this](){
//		changeState("mainMenuScene");
//	}),DelayTime::create(5.f), CallFunc::create([this](){
//		changeState("battleScene");
//	}),DelayTime::create(5.f), CallFunc::create([this](){
//		changeState("mainMenuScene");
//	}),DelayTime::create(5.f), CallFunc::create([this](){
//		changeState("battleScene");
//	}),DelayTime::create(5.f), CallFunc::create([this](){
//		changeState("mainMenuScene");
//	}),DelayTime::create(5.f), CallFunc::create([this](){
//		changeState("battleScene");
//	}),DelayTime::create(5.f), CallFunc::create([this](){
//		changeState("mainMenuScene");
//	}),DelayTime::create(5.f), CallFunc::create([this](){
//		changeState("battleScene");
//	}),DelayTime::create(5.f), CallFunc::create([this](){
//		changeState("mainMenuScene");
//	}), nullptr);
//	mainSceneIns->runAction(seq);

}

void gameManager::changeState(const std::string& state) {
	if (mainSceneIns == nullptr) {
		LOG_ERROR("gameManager::changeState Instance mainSceneIns is null!");
		return;
	}
	currentState = state;
	mainSceneIns->setRoom(currentState);
}
