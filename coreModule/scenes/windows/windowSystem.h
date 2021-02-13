#ifndef COMMON_WINDOWSYSTEM_H
#define COMMON_WINDOWSYSTEM_H

#include "cocos2d.h"
#include "common/coreModule/nodes/nodeProperties.h"

namespace common::coreModule {

	class windowSystem : public cocos2d::Node {
	public:
		enum class eWindowState {
			CLOSED = 0,
			OPENING,
			OPENED,
			CLOSING
		};
		windowSystem();
		~windowSystem() = default;

		CREATE_FUNC(windowSystem);

	private:
		eWindowState currentState = eWindowState::CLOSED;
	};
}//cardsApp::interfaceModule


#endif //COMMON_WINDOWSYSTEM_H
