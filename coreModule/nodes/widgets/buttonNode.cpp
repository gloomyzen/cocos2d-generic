#include "buttonNode.h"
#include "cocos-ext.h"
#include "common/utilityModule/covertUtility.h"

using namespace common::coreModule;

buttonNode::buttonNode() {
    initListener();
    setPressedActionEnabled(false);
}

buttonNode::~buttonNode() {}

void buttonNode::initListener() {
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
    });
}

void buttonNode::adaptRenderers() {
    if (_buttonNormalRenderer)
        Button::adaptRenderers();
}
void buttonNode::onPressStateChangedToNormal() {
    if (allowSizeAction)
        Button::onPressStateChangedToNormal();
}
void buttonNode::onPressStateChangedToPressed() {
    if (allowSizeAction)
        Button::onPressStateChangedToPressed();
}

void buttonNode::onPressStateChangedToDisabled() {
    if (allowSizeAction)
        Button::onPressStateChangedToDisabled();
}

bool buttonNode::init() {
    auto init = Button::init();
    if (_buttonNormalRenderer) removeProtectedChild(_buttonNormalRenderer, true);
    if (_buttonClickedRenderer) removeProtectedChild(_buttonClickedRenderer, true);
    if (_buttonDisabledRenderer) removeProtectedChild(_buttonDisabledRenderer, true);
    Button::initRenderer();
    return init;
}
