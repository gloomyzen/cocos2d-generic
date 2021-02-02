#ifndef COMMON_SOUNDBUTTON_H
#define COMMON_SOUNDBUTTON_H

#include "cocos2d.h"
#include "common/coreModule/nodes/nodeProperties.h"
#include "common/coreModule/nodes/widgets/eventNode.h"
#include <string>
#include <functional>

namespace common {
	namespace coreModule {

		class soundButton : public nodeProperties<cocos2d::Sprite>, public common::coreModule::eventNode {
			enum class eSoundButtonStatus {
				START_CLICK = 0,
				END_CLICK,
			};
		public:
			soundButton();
			~soundButton() override;
			CREATE_FUNC(soundButton);

			static Node *createNode() {
				return soundButton::create();
			}

			bool getAllowSpamTap() const { return allowSpamTap; }
			void setAllowSpamTap(bool value) { allowSpamTap = value; }
		private:
			void initListener();
			std::function<void()> soundCallback = nullptr;
			cocos2d::EventListenerTouchOneByOne* listener = nullptr;
			bool allowSpamTap = false;
		};

	}
}

#endif //COMMON_SOUNDBUTTON_H
