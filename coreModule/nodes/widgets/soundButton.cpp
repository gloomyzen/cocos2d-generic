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
		lastEvent = getTouchCollided(touch, this);
		if (lastEvent == eventNode::eEventAction::COLLIDE) {
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
		} else if (lastEvent == eventNode::eEventAction::COLLIDE_CHILD) {
			return true;
		}

		return false;
	};
	listener->onTouchEnded = [this](cocos2d::Touch* touch, cocos2d::Event* event){
		if (!clickable || lastEvent != eventNode::eEventAction::COLLIDE)
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

eventNode::eEventAction soundButton::getTouchCollided(cocos2d::Touch* touch, cocos2d::Node* node) {
	auto touchLocation = node->convertToNodeSpace(touch->getLocation());
	auto sprite = dynamic_cast<cocos2d::Sprite*>(node);
	auto correctNode = eventNode::eEventAction::NO_MATCHING;
	if (sprite != nullptr && (sprite->getRenderMode() == RenderMode::QUAD_BATCHNODE || sprite->getRenderMode() == RenderMode::POLYGON)) {
		correctNode = sprite->getTextureRect().containsPoint(touchLocation) ? eventNode::eEventAction::COLLIDE : eventNode::eEventAction::NO_MATCHING;
	} else {
		auto rect = cocos2d::Rect(0, 0, node->getContentSize().width, node->getContentSize().height);
		correctNode = rect.containsPoint(touchLocation) ? eventNode::eEventAction::COLLIDE : eventNode::eEventAction::NO_MATCHING;
	}
	if ((correctNode == eventNode::eEventAction::COLLIDE || correctNode == eventNode::eEventAction::COLLIDE_CHILD) && !node->getChildren().empty()) {
		for (const auto& item : node->getChildren()) {
			auto childCheck = getTouchCollided(touch, item);
			if (childCheck == eventNode::eEventAction::COLLIDE || childCheck == eventNode::eEventAction::COLLIDE_CHILD) {
				return eventNode::eEventAction::COLLIDE_CHILD;
			}
		}
	}

	return correctNode;
}

