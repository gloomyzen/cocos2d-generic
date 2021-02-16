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
		if (getTouchCollided(touch, this)) {
			if (!clickable)
				return false;

			auto currentAction = getActionByTag(static_cast<int>(soundButton::eSoundButtonStatus::END_CLICK));
			if (currentAction != nullptr && !getAllowSpamTap() && !currentAction->isDone()) {
				return false;
			} else if (currentAction != nullptr && getAllowSpamTap() && !currentAction->isDone()) {
				if (onTouchBegan)
					onTouchBegan(touch, event);
			}

			if (soundCallback)
				soundCallback();

			auto clickAction = cocos2d::TintTo::create(0.1f, cocos2d::Color3B(235, 235, 235));
			auto clb = cocos2d::CallFunc::create([this, touch, event](){
				if (onTouchBegan)
					onTouchBegan(touch, event);
			});
			auto seq = cocos2d::Sequence::create(clickAction, clb, nullptr);
			seq->setTag(static_cast<int>(soundButton::eSoundButtonStatus::START_CLICK));
			runAction(seq);

			return true;
		}

		return false;
	};
	listener->onTouchEnded = [this](cocos2d::Touch* touch, cocos2d::Event* event){
		if (!clickable)
			return false;

		auto fadeOut = cocos2d::TintTo::create(0.1f, cocos2d::Color3B(255, 255, 255));
		auto clb = cocos2d::CallFunc::create([this, touch, event](){
			if (onTouchEnded)
				onTouchEnded(touch, event);
		});

		auto currentAction = getActionByTag(static_cast<int>(soundButton::eSoundButtonStatus::START_CLICK));
		auto actionSeq = dynamic_cast<cocos2d::Sequence*>(currentAction);
		if (actionSeq != nullptr && !actionSeq->isDone()) {
			auto seq = cocos2d::Sequence::create(actionSeq, fadeOut, clb, nullptr);
			seq->setTag(static_cast<int>(soundButton::eSoundButtonStatus::END_CLICK));
			runAction(seq);
		} else {
			auto seq = cocos2d::Sequence::create(fadeOut, clb, nullptr);
			seq->setTag(static_cast<int>(soundButton::eSoundButtonStatus::END_CLICK));
			runAction(seq);
		}

		return true;
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

bool soundButton::getTouchCollided(cocos2d::Touch* touch, cocos2d::Node* node) {
	//todo return enum
	auto touchLocation = node->convertToNodeSpace(touch->getLocation());
	auto sprite = dynamic_cast<cocos2d::Sprite*>(node);
	bool correctNode;
	if (sprite != nullptr && (sprite->getRenderMode() == RenderMode::QUAD_BATCHNODE || sprite->getRenderMode() == RenderMode::POLYGON)) {
		correctNode = sprite->getTextureRect().containsPoint(touchLocation);
	} else {
		auto rect = cocos2d::Rect(0, 0, node->getContentSize().width, node->getContentSize().height);
		correctNode = rect.containsPoint(touchLocation);
	}
	if (correctNode && !node->getChildren().empty()) {
		for (const auto& item : node->getChildren()) {
			auto childCheck = getTouchCollided(touch, item);
			if (childCheck) {
				return false;
			}
		}
	}

	return correctNode;
}

