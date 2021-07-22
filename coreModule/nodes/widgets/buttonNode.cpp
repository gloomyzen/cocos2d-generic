#include "buttonNode.h"
#include "generic/utilityModule/covertUtility.h"
#include "generic/utilityModule/stringUtility.h"

using namespace generic::coreModule;

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
            if ((currentAction != nullptr && !getAllowSpamTap() && !currentAction->isDone()) || !getAllowClick()) {
                return false;
            }

            if (auto clb = getOnTouchBegan()) {
                clb();
            }

            defaultColor = getColor();
            if (changeColorByClick) {
                auto nextColor = utilityModule::convertUtility::changeColorByPercent(defaultColor, 0.93);
                auto clickAction = cocos2d::TintTo::create(0.01f, nextColor);
                auto seq = cocos2d::Sequence::create(clickAction, nullptr);
                seq->setTag(static_cast<int>(buttonNode::eButtonStatus::START_CLICK));
                runAction(seq);
            }
            moveTimes = 0;
        } break;
        case ui::Widget::TouchEventType::MOVED: {
            ++moveTimes;
        } break;
        case ui::Widget::TouchEventType::ENDED:
        case ui::Widget::TouchEventType::CANCELED: {

            if (!getAllowClick()) {
                return false;
            }


            auto currentAction = getActionByTag(static_cast<int>(buttonNode::eButtonStatus::START_CLICK));
            auto actionSeq = dynamic_cast<cocos2d::Sequence*>(currentAction);
            auto clb = cocos2d::CallFunc::create([this, type]() {
                if (auto fn = getOnTouchEnded()) {
                    if (type == ui::Widget::TouchEventType::ENDED && moveTimes < 8) {
                        fn();
                    }
                }
            });
            if (changeColorByClick) {
                auto fadeOut = cocos2d::TintTo::create(0.1f, defaultColor);
                if (actionSeq != nullptr && !actionSeq->isDone()) {
                    auto seq = cocos2d::Sequence::create(actionSeq, fadeOut, clb, nullptr);
                    seq->setTag(static_cast<int>(buttonNode::eButtonStatus::END_CLICK));
                    runAction(seq);
                } else {
                    auto seq = cocos2d::Sequence::create(fadeOut, clb, nullptr);
                    seq->setTag(static_cast<int>(buttonNode::eButtonStatus::END_CLICK));
                    runAction(seq);
                }
            } else {
                if (actionSeq != nullptr && !actionSeq->isDone()) {
                    auto seq = cocos2d::Sequence::create(actionSeq, clb, nullptr);
                    seq->setTag(static_cast<int>(buttonNode::eButtonStatus::END_CLICK));
                    runAction(seq);
                } else {
                    auto seq = cocos2d::Sequence::create(clb, nullptr);
                    seq->setTag(static_cast<int>(buttonNode::eButtonStatus::END_CLICK));
                    runAction(seq);
                }
            }
            return true;
        } break;
        default:
            break;
        }
        return true;
    });
}
