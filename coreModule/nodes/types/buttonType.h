#ifndef GENERIC_BUTTONTYPE_H
#define GENERIC_BUTTONTYPE_H

#include "cocos2d.h"
#include "generic/coreModule/nodes/types/eventNode.h"
#include <functional>
#include <string>

namespace generic::coreModule {

    template<typename T>
    class buttonType
      : public T
      , public generic::coreModule::eventNode {

        enum class eButtonStatus {
            START_CLICK = 0,
            END_CLICK,
        };

    public:

        ~buttonType() {}
        void initListener();
        bool getAllowSpamTap() const { return allowSpamTap; }
        void setAllowSpamTap(bool value) { allowSpamTap = value; }
        bool getAllowClick() const { return allowClick; }
        void setAllowClick(bool value) { allowClick = value; }

    private:
        cocos2d::EventListenerTouchOneByOne* event = nullptr;
        bool allowSpamTap = false;
        bool allowClick = true;
        bool blockedByShape = false;

    protected:
        cocos2d::Color3B defaultColor;
        int moveTimes;
    };
}


#endif// GENERIC_BUTTONTYPE_H
