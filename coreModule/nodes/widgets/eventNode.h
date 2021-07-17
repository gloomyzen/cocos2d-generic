#ifndef GENERIC_TOUCHESNODE_H
#define GENERIC_TOUCHESNODE_H

#include "cocos2d.h"
#include <functional>
#include <utility>

namespace generic::coreModule {
    class eventNode {
    public:
        enum class eEventAction { COLLIDE = 0, NO_MATCHING, COLLIDE_CHILD };
        typedef std::function<void()> eventTouchClb;
        eventNode() {}
        virtual ~eventNode() = default;

        virtual void setClickable(bool value) {
            clickable = value;
        }
        virtual bool isClickable() const {
            return clickable;
        }
        virtual void setOnTouchBegan(eventTouchClb clb) {
            touchBeganClb = std::move(clb);
        }
        virtual eventTouchClb getOnTouchBegan() {
            return touchBeganClb;
        }
        virtual void setOnTouchEnded(eventTouchClb clb) {
            touchEndedClb = std::move(clb);
        }
        virtual eventTouchClb getOnTouchEnded() {
            return touchEndedClb;
        }


    protected:
        eventTouchClb touchBeganClb = nullptr;
        eventTouchClb touchEndedClb = nullptr;
        bool clickable = true;
    };
}// namespace generic::coreModule
#endif// GENERIC_TOUCHESNODE_H
