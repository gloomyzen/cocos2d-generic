#include "gameManager.h"
#include "common/coreModule/scenes/mainScene.h"
#include "common/debugModule/logManager.h"

using namespace common;
using namespace common::coreModule;

gameManager *currentGameManager = nullptr;

gameManager::gameManager() {
	currentState = eGameStates::LOADING_SCREEN;
	mainSceneIns = dynamic_cast<mainScene *>(mainScene::createScene());
	listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](cocos2d::Touch* touch, cocos2d::Event* event){
		return true;
	};
	listener->onTouchEnded = [](cocos2d::Touch* touch, cocos2d::Event* event){
		return true;
	};
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, mainSceneIns);
};

gameManager::~gameManager() {
	cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(listener);
}

gameManager &gameManager::getInstance() {
	if (currentGameManager == nullptr) {
		currentGameManager = new gameManager();
	}
	return *currentGameManager;
}

void gameManager::run(eGameStates state) {
	/// Starting from state
	changeState(state);
	Director::getInstance()->runWithScene(mainSceneIns);

	//todo remove after testing
//	auto seq = Sequence::create(DelayTime::create(7.f), CallFunc::create([this](){
//		changeState(eGameStates::BATTLE_SCENE);
//	}), nullptr);
//	mainSceneIns->runAction(seq);

//	auto seq = Sequence::create(DelayTime::create(7.f), CallFunc::create([this](){
//		changeState(eGameStates::BATTLE_SCENE);
//	}),DelayTime::create(5.f), CallFunc::create([this](){
//		changeState(eGameStates::MAIN_MENU);
//	}),DelayTime::create(5.f), CallFunc::create([this](){
//		changeState(eGameStates::BATTLE_SCENE);
//	}),DelayTime::create(5.f), CallFunc::create([this](){
//		changeState(eGameStates::MAIN_MENU);
//	}),DelayTime::create(5.f), CallFunc::create([this](){
//		changeState(eGameStates::BATTLE_SCENE);
//	}),DelayTime::create(5.f), CallFunc::create([this](){
//		changeState(eGameStates::MAIN_MENU);
//	}),DelayTime::create(5.f), CallFunc::create([this](){
//		changeState(eGameStates::BATTLE_SCENE);
//	}),DelayTime::create(5.f), CallFunc::create([this](){
//		changeState(eGameStates::MAIN_MENU);
//	}),DelayTime::create(5.f), CallFunc::create([this](){
//		changeState(eGameStates::BATTLE_SCENE);
//	}),DelayTime::create(5.f), CallFunc::create([this](){
//		changeState(eGameStates::MAIN_MENU);
//	}),DelayTime::create(5.f), CallFunc::create([this](){
//		changeState(eGameStates::BATTLE_SCENE);
//	}),DelayTime::create(5.f), CallFunc::create([this](){
//		changeState(eGameStates::MAIN_MENU);
//	}), nullptr);
//	mainSceneIns->runAction(seq);

}

void gameManager::changeState(eGameStates state) {
	if (mainSceneIns == nullptr) {
		LOG_ERROR("gameManager::changeState Instance mainSceneIns is null!");
		return;
	}
	currentState = state;
	mainSceneIns->setRoom(currentState);
}
