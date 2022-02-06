#ifndef GENERIC_BUTTONTYPE_H
#define GENERIC_BUTTONTYPE_H

#include "cocos/ui/CocosGUI.h"
#include "cocos2d.h"
#include "generic/coreModule/nodes/types/eventNode.h"
#include "generic/coreModule/scenes/sceneInterface.h"
#include "generic/coreModule/scenes/scenesFactoryInstance.h"
#include "generic/utilityModule/convertUtility.h"
#include <functional>
#include <string>

namespace generic::coreModule {

    template<typename T>
    class buttonType
      : public T
      , public generic::coreModule::eventNode {

        enum class eButtonStatus {
            START_CLICK = 0,
            END_CLICK,
        };

    public:

        ~buttonType() {}
        void initListener() {
            if (auto node = dynamic_cast<cocos2d::Node*>(this)) {
                node->setCascadeColorEnabled(true);
            }
            listener = cocos2d::EventListenerTouchOneByOne::create();
            listener->setSwallowTouches(true);
            listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event){
                auto node = event->getCurrentTarget();
                if (auto body = node->getPhysicsBody()) {
                    auto touchPos = touch->getStartLocation();
                    auto shapes = body->getShapes();
                    auto find = std::find_if(shapes.begin(), shapes.end(), [&](cocos2d::PhysicsShape* shape){
                        return shape->containsPoint(touchPos);
                    });
                    if (!shapes.empty() && find == shapes.end()) {
                        return false;
                    }
                } else {
                    auto touchLocation = node->convertToNodeSpace(touch->getLocation());
                    auto boundingBox = node->getBoundingBox();
                    boundingBox.origin.x += boundingBox.size.width * node->getAnchorPoint().x;
                    boundingBox.origin.y += boundingBox.size.height * node->getAnchorPoint().y;
                    if (node->getParent()) {
                        boundingBox.origin.x -= node->getParent()->getContentSize().width * node->getPivotPoint().x;
                        boundingBox.origin.y -= node->getParent()->getContentSize().height * node->getPivotPoint().y;
                    }
                    bool correctNode = boundingBox.containsPoint(touchLocation);
                    if (!correctNode)
                        return false;
                }
                auto currentAction = node->getActionByTag(static_cast<int>(buttonType::eButtonStatus::END_CLICK));
                if ((currentAction != nullptr && !getAllowSpamTap() && !currentAction->isDone()) || !getAllowClick()) {
                    return false;
                }

                if (auto clb = getOnTouchBegan()) {
                    clb();
                }

                defaultColor = node->getColor();
                if (changeColorByClick) {
                    auto nextColor = utilityModule::convertUtility::changeColorByPercent(defaultColor, 0.93);
                    auto clickAction = cocos2d::TintTo::create(0.01f, nextColor);
                    auto seq = cocos2d::Sequence::create(clickAction, nullptr);
                    seq->setTag(static_cast<int>(buttonType::eButtonStatus::START_CLICK));
                    node->runAction(seq);
                } else {
                    auto waitAction = cocos2d::DelayTime::create(0.01f);
                    auto seq = cocos2d::Sequence::create(waitAction, nullptr);
                    seq->setTag(static_cast<int>(buttonType::eButtonStatus::START_CLICK));
                    node->runAction(seq);
                }
                moveTimes = 0;
                return true;
            };
            listener->onTouchMoved = [this](cocos2d::Touch* touch, cocos2d::Event* event){
                ++moveTimes;
                return true;
            };
            listener->onTouchCancelled = listener->onTouchEnded = [this](cocos2d::Touch* touch, cocos2d::Event* event){
                auto node = event->getCurrentTarget();
                if (!getAllowClick()) {
                    return false;
                }

                auto currentAction = node->getActionByTag(static_cast<int>(buttonType::eButtonStatus::START_CLICK));
                auto actionSeq = dynamic_cast<cocos2d::Sequence*>(currentAction);
                auto clb = cocos2d::CallFunc::create([this]() {
                    if (auto fn = getOnTouchEnded()) {
                        if (moveTimes < 8) {
                            fn();
                        }
                    }
                });
                if (changeColorByClick) {
                    auto fadeOut = cocos2d::TintTo::create(0.1f, defaultColor);
                    if (actionSeq != nullptr && !actionSeq->isDone()) {
                        auto seq = cocos2d::Sequence::create(actionSeq, fadeOut, clb, nullptr);
                        seq->setTag(static_cast<int>(buttonType::eButtonStatus::END_CLICK));
                        node->runAction(seq);
                    } else {
                        auto seq = cocos2d::Sequence::create(fadeOut, clb, nullptr);
                        seq->setTag(static_cast<int>(buttonType::eButtonStatus::END_CLICK));
                        node->runAction(seq);
                    }
                } else {
                    if (actionSeq != nullptr && !actionSeq->isDone()) {
                        auto seq = cocos2d::Sequence::create(actionSeq, clb, nullptr);
                        seq->setTag(static_cast<int>(buttonType::eButtonStatus::END_CLICK));
                        node->runAction(seq);
                    } else {
                        auto seq = cocos2d::Sequence::create(clb, nullptr);
                        seq->setTag(static_cast<int>(buttonType::eButtonStatus::END_CLICK));
                        node->runAction(seq);
                    }
                }
                return true;
            };
            GET_CURRENT_SCENE()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
        }
        bool getAllowSpamTap() const { return allowSpamTap; }
        void setAllowSpamTap(bool value) { allowSpamTap = value; }
        bool getAllowClick() const { return allowClick; }
        void setAllowClick(bool value) { allowClick = value; }
        void setChangeColorByClick(bool value) { changeColorByClick = value; }

    private:
        cocos2d::EventListenerTouchOneByOne* listener = nullptr;
        bool allowSpamTap = false;
        bool allowClick = true;
        bool changeColorByClick = true;

    protected:
        cocos2d::Color3B defaultColor;
        int moveTimes = 0;
    };
}


#endif// GENERIC_BUTTONTYPE_H
