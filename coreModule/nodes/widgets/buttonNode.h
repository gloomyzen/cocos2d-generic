#ifndef MERCENARY_BATTLES_BUTTONNODE_H
#define MERCENARY_BATTLES_BUTTONNODE_H

#include "cocos/ui/CocosGUI.h"
#include "cocos2d.h"
#include "common/coreModule/nodes/nodeProperties.h"
#include "common/coreModule/nodes/widgets/eventNode.h"
#include <functional>
#include <string>

namespace common::coreModule {

    class buttonNode
        : public nodeProperties
        , public cocos2d::ui::Widget
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
        void loadTexture(const std::string&);

      private:
        void initListener();
        bool allowSpamTap = false;

      protected:
        cocos2d::Color3B defaultColor;
        cocos2d::Sprite* sprite = nullptr;
    };
}// namespace common::coreModule


#endif// MERCENARY_BATTLES_BUTTONNODE_H
