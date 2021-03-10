#include "soundButton.h"
#include "common/coreModule/gameManager.h"
#include "common/debugModule/logManager.h"
#include "editor-support/cocostudio/SimpleAudioEngine.h"
#include <utility>
#include "common/utilityModule/covertUtility.h"

using namespace common::coreModule;

soundButton::soundButton() {
    listener = cocos2d::EventListenerTouchOneByOne::create();
    soundCallback = []() { CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/click.wav"); };
    initListener();
    bgNode = this;
}

soundButton::~soundButton() { this->getEventDispatcher()->removeEventListener(listener); }

void soundButton::initListener() {
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
        lastEvent = getTouchCollided(touch, this);
        if (lastEvent == eventNode::eEventAction::COLLIDE) {
            if (!clickable)
                return true;
            firstTouchPos = touch->getLocation();
            touchValid = true;

            auto currentAction = bgNode->getActionByTag(static_cast<int>(soundButton::eSoundButtonStatus::END_CLICK));
            if (currentAction != nullptr && !getAllowSpamTap() && !currentAction->isDone()) {
                return false;
            }

            if (soundCallback)
                soundCallback();

            defaultColor = bgNode->getColor();
            auto nextColor = utilityModule::convertUtility::changeColorByPercent(defaultColor, 0.93);
            auto clickAction = cocos2d::TintTo::create(0.1f, nextColor);
            auto seq = cocos2d::Sequence::create(clickAction, nullptr);
            seq->setTag(static_cast<int>(soundButton::eSoundButtonStatus::START_CLICK));
            bgNode->runAction(seq);

            return true;
        } else if (lastEvent == eventNode::eEventAction::COLLIDE_CHILD) {
            return true;
        }

        return false;
    };
    listener->onTouchEnded = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
        if (!clickable || lastEvent != eventNode::eEventAction::COLLIDE)
            return false;
        auto afterEvent = getTouchCollided(touch, this);

        auto fadeOut = cocos2d::TintTo::create(0.1f, defaultColor);

        auto currentAction = bgNode->getActionByTag(static_cast<int>(soundButton::eSoundButtonStatus::START_CLICK));
        auto actionSeq = dynamic_cast<cocos2d::Sequence*>(currentAction);
        if (afterEvent == eventNode::eEventAction::COLLIDE && touchValid) {
            auto clb = cocos2d::CallFunc::create([this, touch, event]() {
                if (onTouch)
                    onTouch(touch, event);
            });
            if (actionSeq != nullptr && !actionSeq->isDone()) {
                auto seq = cocos2d::Sequence::create(actionSeq, fadeOut, clb, nullptr);
                seq->setTag(static_cast<int>(soundButton::eSoundButtonStatus::END_CLICK));
                bgNode->runAction(seq);
            } else {
                auto seq = cocos2d::Sequence::create(fadeOut, clb, nullptr);
                seq->setTag(static_cast<int>(soundButton::eSoundButtonStatus::END_CLICK));
                bgNode->runAction(seq);
            }
        } else {
            if (actionSeq != nullptr && !actionSeq->isDone()) {
                auto seq = cocos2d::Sequence::create(actionSeq, fadeOut, nullptr);
                seq->setTag(static_cast<int>(soundButton::eSoundButtonStatus::END_CLICK));
                bgNode->runAction(seq);
            } else {
                auto seq = cocos2d::Sequence::create(fadeOut, nullptr);
                seq->setTag(static_cast<int>(soundButton::eSoundButtonStatus::END_CLICK));
                bgNode->runAction(seq);
            }
            return false;
        }

        return true;
    };

    listener->onTouchMoved = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
        auto width = 15.f;
        auto touchLoc = touch->getLocation();
        if (!(touchLoc.x < firstTouchPos.x + width && touchLoc.x + width > firstTouchPos.x
              && touchLoc.y < firstTouchPos.y + width && touchLoc.y + width > firstTouchPos.y)) {
            touchValid = false;
        }
        return true;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

eventNode::eEventAction soundButton::getTouchCollided(cocos2d::Touch* touch, cocos2d::Node* node) {
    auto btn = dynamic_cast<soundButton*>(node);
    if (!btn) {
        return eventNode::eEventAction::NO_MATCHING;
    }
    auto touchLocation = node->convertToNodeSpace(touch->getLocation());
    auto sprite = dynamic_cast<cocos2d::Sprite*>(node);
    auto correctNode = eventNode::eEventAction::NO_MATCHING;
    if (sprite != nullptr
        && (sprite->getRenderMode() == RenderMode::QUAD_BATCHNODE || sprite->getRenderMode() == RenderMode::POLYGON)) {
        correctNode = sprite->getTextureRect().containsPoint(touchLocation) ? eventNode::eEventAction::COLLIDE
                                                                            : eventNode::eEventAction::NO_MATCHING;
    } else {
        auto rect = cocos2d::Rect(0, 0, node->getContentSize().width, node->getContentSize().height);
        correctNode =
            rect.containsPoint(touchLocation) ? eventNode::eEventAction::COLLIDE : eventNode::eEventAction::NO_MATCHING;
    }
    if ((correctNode == eventNode::eEventAction::COLLIDE || correctNode == eventNode::eEventAction::COLLIDE_CHILD)
        && !node->getChildren().empty()) {
        for (const auto& item : node->getChildren()) {
            auto childCheck = getTouchCollided(touch, item);
            if (childCheck == eventNode::eEventAction::COLLIDE
                || childCheck == eventNode::eEventAction::COLLIDE_CHILD) {
                return eventNode::eEventAction::COLLIDE_CHILD;
            }
        }
    }

    return correctNode;
}

void soundButton::setSwallowTouches(bool value) { listener->setSwallowTouches(value); }
