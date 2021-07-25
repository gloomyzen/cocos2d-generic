#ifndef GENERIC_SOUNDBUTTON_H
#define GENERIC_SOUNDBUTTON_H

#include "cocos2d.h"
#include "generic/coreModule/nodes/nodeProperties.h"
#include "generic/coreModule/nodes/types/buttonNode.h"
#include "generic/coreModule/nodes/types/eventNode.h"
#include <functional>
#include <utility>

namespace generic::coreModule {

    class soundButton : public generic::coreModule::buttonNode {
    public:
        soundButton();
        ~soundButton() override;
        CREATE_FUNC(soundButton);

        void setSoundCallback(std::function<void()> value) {
            soundCallback = std::move(value);
        }
        eventTouchClb getOnTouchBegan() override;

    protected:
        std::function<void()> soundCallback = nullptr;
    };

}// namespace generic::coreModule

#endif// GENERIC_SOUNDBUTTON_H
