#ifndef SWIPE_RPG_TOUCHESNODE_H
#define SWIPE_RPG_TOUCHESNODE_H

#include "cocos2d.h"
#include <functional>

namespace common {

	namespace coreModule {

		class eventNode {
			typedef std::function<bool(cocos2d::Touch* touch, cocos2d::Event* event)> eventTouchClb;
		public:
			void setClickable(bool value) { clickable = value; }
			bool isClickable() const { return clickable; }
			void setOnTouchBegan(eventTouchClb clb) { onTouchBegan = clb; }
			eventTouchClb getOnTouchBegan() { return onTouchBegan; }
			void setOnTouchEnded(eventTouchClb clb) { onTouchEnded = clb; }
			eventTouchClb getOnTouchEnded() { return onTouchEnded; }


		protected:

			eventTouchClb onTouchBegan = nullptr;
			eventTouchClb onTouchEnded = nullptr;
			bool clickable = true;

		};
	}
}
#endif //SWIPE_RPG_TOUCHESNODE_H
