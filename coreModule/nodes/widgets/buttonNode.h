#ifndef MERCENARY_BATTLES_BUTTONNODE_H
#define MERCENARY_BATTLES_BUTTONNODE_H

#include "cocos/ui/CocosGUI.h"
#include "cocos2d.h"
#include "common/coreModule/nodes/nodeProperties.h"
#include "common/coreModule/nodes/widgets/eventNode.h"
#include "common/coreModule/nodes/widgets/buttonBase.h"
#include <functional>
#include <string>

namespace common::coreModule {

    class buttonNode
        : public common::coreModule::nodeProperties
        , public common::coreModule::buttonBase
        , public common::coreModule::eventNode {
        enum class eButtonStatus {
            START_CLICK = 0,
            END_CLICK,
        };

      public:
        buttonNode();
        ~buttonNode() override;
        CREATE_FUNC(buttonNode);

        bool getAllowSpamTap() const { return allowSpamTap; }
        void setAllowSpamTap(bool value) { allowSpamTap = value; }

      private:
        void initListener();
        bool allowSpamTap = false;

      protected:
        cocos2d::Color3B defaultColor;
    };
}// namespace common::coreModule


#endif// MERCENARY_BATTLES_BUTTONNODE_H
