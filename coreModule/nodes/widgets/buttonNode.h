#ifndef MERCENARY_BATTLES_BUTTONNODE_H
#define MERCENARY_BATTLES_BUTTONNODE_H

#include "cocos/ui/UIButton.h"
#include "cocos2d.h"
#include "common/coreModule/nodes/nodeProperties.h"
#include "common/coreModule/nodes/widgets/eventNode.h"
#include <functional>
#include <string>

namespace common::coreModule {

    class buttonNode
        : public nodeProperties
        , public cocos2d::ui::Button
        , public common::coreModule::eventNode {
        enum class eButtonStatus {
            START_CLICK = 0,
            END_CLICK,
        };
      public:
        buttonNode();
        ~buttonNode();
        CREATE_FUNC(buttonNode);

        bool getAllowSpamTap() const { return allowSpamTap; }
        void setAllowSpamTap(bool value) { allowSpamTap = value; }

      private:
        void initListener();
        std::function<void()> soundCallback = nullptr;
        bool allowSpamTap = false;

      protected:
        void adaptRenderers() override;

        cocos2d::Color3B defaultColor;
    };
}// namespace common::coreModule


#endif// MERCENARY_BATTLES_BUTTONNODE_H
