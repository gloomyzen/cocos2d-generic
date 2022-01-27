#include "soundButton.h"
#include "generic/audioModule/audioEngineInstance.h"

using namespace generic::coreModule;

soundButton::soundButton() {
    soundCallback = []() {
        GET_AUDIO_ENGINE().play("ui.click");
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
