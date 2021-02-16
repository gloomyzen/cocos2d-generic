#include "soundButton.h"
#include "editor-support/cocostudio/SimpleAudioEngine.h"
#include <utility>
#include "common/coreModule/gameManager.h"

using namespace common::coreModule;

soundButton::soundButton() {
	soundCallback = [](){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/click.wav");
	};
	initListener();
}

soundButton::~soundButton() {
//	this->getEventDispatcher()->removeEventListener(listener);
}

void soundButton::initListener() {
	auto listener = GET_GAME_MANAGER().getListener();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [](cocos2d::Touch* touch, cocos2d::Event* event){
		auto target = dynamic_cast<soundButton*>(event->getCurrentTarget());
		if (!target)
			return false;
		auto touchLocation = target->convertToNodeSpace(touch->getLocation());
		bool correctNode;
		if (target->getRenderMode() == RenderMode::QUAD_BATCHNODE || target->getRenderMode() == RenderMode::POLYGON) {
			correctNode = target->getTextureRect().containsPoint(touchLocation);
		} else {
			auto rect = cocos2d::Rect(0, 0, target->getContentSize().width, target->getContentSize().height);
			correctNode = rect.containsPoint(touchLocation);
		}
		if (correctNode) {
			if (!target->clickable)
				return false;

			auto currentAction = target->getActionByTag(static_cast<int>(soundButton::eSoundButtonStatus::END_CLICK));
			if (currentAction != nullptr && !target->getAllowSpamTap() && !currentAction->isDone()) {
				return false;
			} else if (currentAction != nullptr && target->getAllowSpamTap() && !currentAction->isDone()) {
				if (target->onTouchBegan)
					target->onTouchBegan(touch, event);
			}

			if (target->soundCallback)
				target->soundCallback();

			auto clickAction = cocos2d::TintTo::create(0.1f, cocos2d::Color3B(235, 235, 235));
			auto clb = cocos2d::CallFunc::create([target, touch, event](){
				if (target->onTouchBegan)
					target->onTouchBegan(touch, event);
			});
			auto seq = cocos2d::Sequence::create(clickAction, clb, nullptr);
			seq->setTag(static_cast<int>(soundButton::eSoundButtonStatus::START_CLICK));
			target->runAction(seq);

		}

		return correctNode;
	};
	listener->onTouchEnded = [](cocos2d::Touch* touch, cocos2d::Event* event){
		auto target = dynamic_cast<soundButton*>(event->getCurrentTarget());
		if (!target)
			return false;
		if (!target->clickable)
			return false;

		auto fadeOut = cocos2d::TintTo::create(0.1f, cocos2d::Color3B(255, 255, 255));
		auto clb = cocos2d::CallFunc::create([target, touch, event](){
			if (target->onTouchEnded)
				target->onTouchEnded(touch, event);
		});

		auto currentAction = target->getActionByTag(static_cast<int>(soundButton::eSoundButtonStatus::START_CLICK));
		auto actionSeq = dynamic_cast<cocos2d::Sequence*>(currentAction);
		if (actionSeq != nullptr && !actionSeq->isDone()) {
			auto seq = cocos2d::Sequence::create(actionSeq, fadeOut, clb, nullptr);
			seq->setTag(static_cast<int>(soundButton::eSoundButtonStatus::END_CLICK));
			target->runAction(seq);
		} else {
			auto seq = cocos2d::Sequence::create(fadeOut, clb, nullptr);
			seq->setTag(static_cast<int>(soundButton::eSoundButtonStatus::END_CLICK));
			target->runAction(seq);
		}

		return true;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

