#ifndef COMMON_SOUNDBUTTON_H
#define COMMON_SOUNDBUTTON_H

#include "cocos2d.h"
#include "common/coreModule/nodes/nodeProperties.h"
#include "common/coreModule/nodes/widgets/buttonNode.h"
#include "common/coreModule/nodes/widgets/eventNode.h"
#include <functional>
#include <utility>

namespace common::coreModule {

    class soundButton : public common::coreModule::buttonNode {
      public:
        soundButton();
        ~soundButton() override;

        void setSoundCallback(std::function<void()> value) { soundCallback = std::move(value); }
        eventTouchClb getOnTouchBegan() override;

      protected:
        std::function<void()> soundCallback = nullptr;
    };

}// namespace common::coreModule

#endif// COMMON_SOUNDBUTTON_H
