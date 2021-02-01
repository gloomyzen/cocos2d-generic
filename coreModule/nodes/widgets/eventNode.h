#ifndef COMMON_TOUCHESNODE_H
#define COMMON_TOUCHESNODE_H

#include "cocos2d.h"
#include <functional>
#include <utility>

namespace common {

	namespace coreModule {

		class eventNode {
			typedef std::function<void(cocos2d::Touch* touch, cocos2d::Event* event)> eventTouchClb;
		public:
			eventNode() {}
			virtual ~eventNode() = default;

			void setClickable(bool value) { clickable = value; }
			bool isClickable() const { return clickable; }
			void setOnTouchBegan(eventTouchClb clb) { onTouchBegan = std::move(clb); }
			eventTouchClb getOnTouchBegan() { return onTouchBegan; }
			void setOnTouchEnded(eventTouchClb clb) { onTouchEnded = std::move(clb); }
			eventTouchClb getOnTouchEnded() { return onTouchEnded; }


		protected:

			eventTouchClb onTouchBegan = nullptr;
			eventTouchClb onTouchEnded = nullptr;
			bool clickable = true;

		};
	}
}
#endif //COMMON_TOUCHESNODE_H
