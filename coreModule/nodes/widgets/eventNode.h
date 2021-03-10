#ifndef COMMON_TOUCHESNODE_H
#define COMMON_TOUCHESNODE_H

#include "cocos2d.h"
#include <functional>
#include <utility>

namespace common {

    namespace coreModule {

        class eventNode {
          public:
            enum class eEventAction { COLLIDE = 0, NO_MATCHING, COLLIDE_CHILD };
            typedef std::function<void(cocos2d::Touch* touch, cocos2d::Event* event)> eventTouchClb;
            eventNode() {}
            virtual ~eventNode() = default;

            void setClickable(bool value) { clickable = value; }
            bool isClickable() const { return clickable; }
            void setOnTouch(eventTouchClb clb) { onTouch = std::move(clb); }
            eventTouchClb getOnTouch() { return onTouch; }


          protected:
            eventTouchClb onTouch = nullptr;
            bool clickable = true;
        };
    }// namespace coreModule
}// namespace common
#endif// COMMON_TOUCHESNODE_H
