#pragma once
#ifndef GENERIC_SOUNDBUTTON_H
#define GENERIC_SOUNDBUTTON_H

#include "axmol.h"
#include "generic/coreModule/nodes/nodeProperties.h"
#include "generic/coreModule/nodes/types/buttonType.h"
#include "generic/coreModule/nodes/types/eventNode.h"
#include <functional>
#include <utility>

namespace generic::coreModule {

    class soundButton
      : public generic::coreModule::buttonType<ax::Sprite>
      , public generic::coreModule::nodeProperties {
    public:
        soundButton();
        ~soundButton() override;

        void setSoundCallback(std::function<void()> value) {
            soundCallback = std::move(value);
        }
        eventTouchClb getOnTouchBegan() override;

    protected:
        std::function<void()> soundCallback = nullptr;
    };

}// namespace generic::coreModule

#endif// GENERIC_SOUNDBUTTON_H
