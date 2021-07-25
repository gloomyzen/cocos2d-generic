#ifndef GENERIC_BUTTONNODE_H
#define GENERIC_BUTTONNODE_H

#include "cocos/ui/CocosGUI.h"
#include "cocos2d.h"
#include "generic/coreModule/nodes/nodeProperties.h"
#include "generic/coreModule/nodes/types/buttonBase.h"
#include "generic/coreModule/nodes/types/eventNode.h"
#include <functional>
#include <string>

namespace generic::coreModule {

    class buttonNode
        : public generic::coreModule::nodeProperties
        , public generic::coreModule::buttonBase
        , public generic::coreModule::eventNode {
        enum class eButtonStatus {
            START_CLICK = 0,
            END_CLICK,
        };

    public:
        buttonNode();
        ~buttonNode() override;
        CREATE_FUNC(buttonNode);

        bool getAllowSpamTap() const {
            return allowSpamTap;
        }
        void setAllowSpamTap(bool value) {
            allowSpamTap = value;
        }

        bool getAllowClick() const {
            return allowClick;
        }
        void setAllowClick(bool value) {
            allowClick = value;
        }

    private:
        void initListener();
        bool allowSpamTap = false;
        bool allowClick = true;

    protected:
        cocos2d::Color3B defaultColor;
        int moveTimes;
    };
}// namespace generic::coreModule


#endif// GENERIC_BUTTONNODE_H
