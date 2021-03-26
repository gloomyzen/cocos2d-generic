#include "buttonNode.h"
#include "common/utilityModule/covertUtility.h"
#include "editor-support/cocostudio/SimpleAudioEngine.h"
#include <utility>

using namespace common::coreModule;

buttonNode::buttonNode() {
    soundCallback = []() { CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/click.wav"); };
    initListener();
}

buttonNode::~buttonNode() {
    //this->getEventDispatcher()->removeEventListener(listener);
}

void buttonNode::initListener() {
    addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
           switch (type)
           {
           case ui::Widget::TouchEventType::BEGAN: {
               auto currentAction = getActionByTag(static_cast<int>(buttonNode::eButtonStatus::END_CLICK));
               if (currentAction != nullptr && !getAllowSpamTap() && !currentAction->isDone()) {
                   return false;
               }

               if (soundCallback)
                   soundCallback();

               defaultColor = getColor();
               auto nextColor = utilityModule::convertUtility::changeColorByPercent(defaultColor, 0.93);
               auto clickAction = cocos2d::TintTo::create(0.1f, nextColor);
               auto seq = cocos2d::Sequence::create(clickAction, nullptr);
               seq->setTag(static_cast<int>(buttonNode::eButtonStatus::START_CLICK));
               runAction(seq);
           }
               break;
           case ui::Widget::TouchEventType::ENDED: {
               auto fadeOut = cocos2d::TintTo::create(0.1f, defaultColor);

               auto currentAction = getActionByTag(static_cast<int>(buttonNode::eButtonStatus::START_CLICK));
               auto actionSeq = dynamic_cast<cocos2d::Sequence*>(currentAction);
               auto clb = cocos2d::CallFunc::create([this]() {
                      if (onTouch)
                          onTouch();
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
           }
               break;
           default:
               break;
           }
    });
}

void buttonNode::adaptRenderers() {
    if (_buttonNormalRenderer)
        Button::adaptRenderers();
}
