#include "soundButton.h"
#include "editor-support/cocostudio/SimpleAudioEngine.h"

using namespace common::coreModule;

soundButton::soundButton() {
    soundCallback = []() { CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/click.wav"); };
}

soundButton::~soundButton() = default;

eventNode::eventTouchClb soundButton::getOnTouchBegan() {
    auto clb = [this]() {
        if (auto fn = eventNode::getOnTouchBegan()) {
            fn();
        }
        if (auto fn = soundCallback) {
            fn();
        }
    };
    return clb;
}
