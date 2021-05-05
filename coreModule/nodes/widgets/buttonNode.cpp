#include "buttonNode.h"
#include "common/utilityModule/covertUtility.h"

using namespace common::coreModule;

buttonNode::buttonNode() {
    setFocusEnabled(false);
    initListener();
}

buttonNode::~buttonNode() = default;

void buttonNode::initListener() {
    setTouchEnabled(true);
    addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type) {
        case ui::Widget::TouchEventType::BEGAN: {
            auto currentAction = getActionByTag(static_cast<int>(buttonNode::eButtonStatus::END_CLICK));
            if (currentAction != nullptr && !getAllowSpamTap() && !currentAction->isDone()) {
                return false;
            }

            if (auto clb = getOnTouchBegan()) {
                clb();
            }

            defaultColor = getColor();
            auto nextColor = utilityModule::convertUtility::changeColorByPercent(defaultColor, 0.93);
            setCascadeColorEnabled(true);
            auto clickAction = cocos2d::TintTo::create(0.01f, nextColor);
            auto seq = cocos2d::Sequence::create(clickAction, nullptr);
            seq->setTag(static_cast<int>(buttonNode::eButtonStatus::START_CLICK));
            runAction(seq);
        } break;
        case ui::Widget::TouchEventType::ENDED:
        case ui::Widget::TouchEventType::CANCELED: {
            auto fadeOut = cocos2d::TintTo::create(0.1f, defaultColor);

            auto currentAction = getActionByTag(static_cast<int>(buttonNode::eButtonStatus::START_CLICK));
            auto actionSeq = dynamic_cast<cocos2d::Sequence*>(currentAction);
            auto clb = cocos2d::CallFunc::create([this, type]() {
                if (auto fn = getOnTouchEnded()) {
                    if (type == ui::Widget::TouchEventType::ENDED) {
                        fn();
                    }
                }
            });
            if (actionSeq != nullptr && !actionSeq->isDone()) {
                auto seq = cocos2d::Sequence::create(actionSeq, fadeOut, clb, nullptr);
                seq->setTag(static_cast<int>(buttonNode::eButtonStatus::END_CLICK));
                runAction(seq);
            } else {
                auto seq = cocos2d::Sequence::create(fadeOut, clb, nullptr);
                seq->setTag(static_cast<int>(buttonNode::eButtonStatus::END_CLICK));
                runAction(seq);
            }
        } break;
        default:
            break;
        }
        return true;
    });
}
