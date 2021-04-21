#include "soundButton.h"
#include "common/audioModule/audioEngine.h"

using namespace common::coreModule;

soundButton::soundButton() {
    soundCallback = []() { GET_AUDIO_ENGINE().play("ui.click");
    };
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
