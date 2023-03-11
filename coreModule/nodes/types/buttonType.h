#pragma once
#ifndef GENERIC_BUTTONTYPE_H
#define GENERIC_BUTTONTYPE_H

#include "ui/CocosGUI.h"
#include "axmol.h"
#include "generic/coreModule/nodes/types/eventNode.h"
#include "generic/coreModule/scenes/sceneInterface.h"
#include "generic/coreModule/scenes/scenesFactoryInstance.h"
#include "generic/coreModule/components/transformComponent.h"
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
            if (auto node = dynamic_cast<ax::Node*>(this)) {
                node->setCascadeColorEnabled(true);
            }
            listener = ax::EventListenerTouchOneByOne::create();
            listener->setSwallowTouches(true);
            listener->onTouchBegan = [this](ax::Touch* touch, ax::Event* event){
                auto node = event->getCurrentTarget();
                if (auto body = node->getPhysicsBody()) {
                    auto touchPos = touch->getStartLocation();
                    auto shapes = body->getShapes();
                    auto find = std::find_if(shapes.begin(), shapes.end(), [&](ax::PhysicsShape* shape){
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
                        if (auto transform = dynamic_cast<transformComponent*>(node->getComponent(transformComponent::TRANSFORM_COMPONENT_NAME))) {
                            boundingBox.origin.x -= node->getParent()->getContentSize().width * transform->getPivotPoint().x;
                            boundingBox.origin.y -= node->getParent()->getContentSize().height * transform->getPivotPoint().y;
                        }
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
                    auto clickAction = ax::TintTo::create(0.01f, nextColor);
                    auto seq = ax::Sequence::create(clickAction, nullptr);
                    seq->setTag(static_cast<int>(buttonType::eButtonStatus::START_CLICK));
                    node->runAction(seq);
                } else {
                    auto waitAction = ax::DelayTime::create(0.01f);
                    auto seq = ax::Sequence::create(waitAction, nullptr);
                    seq->setTag(static_cast<int>(buttonType::eButtonStatus::START_CLICK));
                    node->runAction(seq);
                }
                moveTimes = 0;
                return true;
            };
            listener->onTouchMoved = [this](ax::Touch* touch, ax::Event* event){
                ++moveTimes;
                return true;
            };
            listener->onTouchCancelled = listener->onTouchEnded = [this](ax::Touch* touch, ax::Event* event){
                auto node = event->getCurrentTarget();
                if (!getAllowClick()) {
                    return false;
                }

                auto currentAction = node->getActionByTag(static_cast<int>(buttonType::eButtonStatus::START_CLICK));
                auto actionSeq = dynamic_cast<ax::Sequence*>(currentAction);
                auto clb = ax::CallFunc::create([this]() {
                    if (auto fn = getOnTouchEnded()) {
                        if (moveTimes < 8) {
                            fn();
                        }
                    }
                });
                if (changeColorByClick) {
                    auto fadeOut = ax::TintTo::create(0.1f, defaultColor);
                    if (actionSeq != nullptr && !actionSeq->isDone()) {
                        auto seq = ax::Sequence::create(actionSeq, fadeOut, clb, nullptr);
                        seq->setTag(static_cast<int>(buttonType::eButtonStatus::END_CLICK));
                        node->runAction(seq);
                    } else {
                        auto seq = ax::Sequence::create(fadeOut, clb, nullptr);
                        seq->setTag(static_cast<int>(buttonType::eButtonStatus::END_CLICK));
                        node->runAction(seq);
                    }
                } else {
                    if (actionSeq != nullptr && !actionSeq->isDone()) {
                        auto seq = ax::Sequence::create(actionSeq, clb, nullptr);
                        seq->setTag(static_cast<int>(buttonType::eButtonStatus::END_CLICK));
                        node->runAction(seq);
                    } else {
                        auto seq = ax::Sequence::create(clb, nullptr);
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
        ax::EventListenerTouchOneByOne* listener = nullptr;
        bool allowSpamTap = false;
        bool allowClick = true;
        bool changeColorByClick = true;

    protected:
        ax::Color3B defaultColor;
        int moveTimes = 0;
    };
}


#endif// GENERIC_BUTTONTYPE_H
