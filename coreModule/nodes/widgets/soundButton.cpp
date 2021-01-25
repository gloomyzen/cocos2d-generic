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
	setClickCallback([](){});
}

soundButton::~soundButton() {
	this->getEventDispatcher()->removeEventListener(listener);
	Sprite::~Sprite();
}

void soundButton::setClickCallback(std::function<void()> clb) {
	onClickCallback = std::move(clb);
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event){
		auto touchLocation = convertToNodeSpace(touch->getLocation());
		bool correctNode = getTextureRect().containsPoint(touchLocation);
		if (correctNode) {
			if (!clickable)
				return false;
			auto currentAction = event->getCurrentTarget()->getActionByTag(static_cast<int>(soundButton::eSoundButtonStatus::END_CLICK));
			if (currentAction != nullptr && !allowSpamTap && !currentAction->isDone())
				return false;

			if (soundCallback)
				soundCallback();

			auto clickAction = cocos2d::TintTo::create(0.1f, cocos2d::Color3B(235, 235, 235));
			clickAction->setTag(static_cast<int>(soundButton::eSoundButtonStatus::START_CLICK));
			event->getCurrentTarget()->runAction(clickAction);
		}

		return correctNode;
	};
	listener->onTouchEnded = [this](cocos2d::Touch*, cocos2d::Event* event){
		if (!clickable)
			return false;

		auto fadeOut = cocos2d::TintTo::create(0.1f, cocos2d::Color3B(255, 255, 255));
		auto clb = cocos2d::CallFunc::create([this](){
			if (onClickCallback)
				onClickCallback();
		});

		auto currentAction = event->getCurrentTarget()->getActionByTag(static_cast<int>(soundButton::eSoundButtonStatus::START_CLICK));
		if (currentAction != nullptr && !currentAction->isDone()) {
			auto seq = cocos2d::Sequence::create(dynamic_cast<cocos2d::TintTo*>(currentAction->clone()), fadeOut, clb, nullptr);
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

