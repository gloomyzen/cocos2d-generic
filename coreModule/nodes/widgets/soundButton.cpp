#include "soundButton.h"
#include "editor-support/cocostudio/SimpleAudioEngine.h"
#include <utility>
#include "common/coreModule/gameManager.h"

using namespace common::coreModule;

soundButton::soundButton() {
	listener = cocos2d::EventListenerTouchOneByOne::create();
	soundCallback = [](){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/click.wav");
	};
	initListener();
}

soundButton::~soundButton() {
	this->getEventDispatcher()->removeEventListener(listener);
}

void soundButton::initListener() {
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event){
		auto touchLocation = convertToNodeSpace(touch->getLocation());
		bool correctNode = getTextureRect().containsPoint(touchLocation);
		if (correctNode) {
			if (!clickable)
				return false;

			auto currentAction = event->getCurrentTarget()->getActionByTag(static_cast<int>(soundButton::eSoundButtonStatus::END_CLICK));
			if (currentAction != nullptr && !getAllowSpamTap() && !currentAction->isDone()) {
				return false;
			} else if (currentAction != nullptr && getAllowSpamTap() && !currentAction->isDone()) {
				if (onTouchBegan)
					onTouchBegan(touch, event);
			}

			if (soundCallback)
				soundCallback();

			auto clickAction = cocos2d::TintTo::create(0.1f, cocos2d::Color3B(235, 235, 235));
			auto clb = cocos2d::CallFunc::create([&](){
				if (onTouchBegan)
					onTouchBegan(touch, event);
			});
			auto seq = cocos2d::Sequence::create(clickAction, clb, nullptr);
			seq->setTag(static_cast<int>(soundButton::eSoundButtonStatus::START_CLICK));
			event->getCurrentTarget()->runAction(seq);
		}

		return correctNode;
	};
	listener->onTouchEnded = [this](cocos2d::Touch* touch, cocos2d::Event* event){
		if (!clickable)
			return false;

		auto fadeOut = cocos2d::TintTo::create(0.1f, cocos2d::Color3B(255, 255, 255));
		auto clb = cocos2d::CallFunc::create([&](){
			if (onTouchEnded)
				onTouchEnded(touch, event);
		});

		auto currentAction = event->getCurrentTarget()->getActionByTag(static_cast<int>(soundButton::eSoundButtonStatus::START_CLICK));
		auto actionSeq = dynamic_cast<cocos2d::Sequence*>(currentAction);
		if (actionSeq != nullptr && !actionSeq->isDone()) {
			auto seq = cocos2d::Sequence::create(actionSeq, fadeOut, clb, nullptr);
			seq->setTag(static_cast<int>(soundButton::eSoundButtonStatus::END_CLICK));
			event->getCurrentTarget()->runAction(seq);
		} else {
			auto seq = cocos2d::Sequence::create(fadeOut, clb, nullptr);
			seq->setTag(static_cast<int>(soundButton::eSoundButtonStatus::END_CLICK));
			event->getCurrentTarget()->runAction(seq);
		}

		return true;
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

