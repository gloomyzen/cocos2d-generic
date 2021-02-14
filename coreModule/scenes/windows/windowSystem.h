#ifndef COMMON_WINDOWSYSTEM_H
#define COMMON_WINDOWSYSTEM_H

#include "cocos2d.h"
#include "common/coreModule/nodes/nodeProperties.h"
#include <map>
#include <string>

namespace common::coreModule {
	class windowBase;

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
		void registerWindow(const std::string&, const std::function<windowBase*()>&);

	private:
		eWindowState currentState = eWindowState::CLOSED;
		std::map<std::string, std::function<windowBase*()>> windowList;
	};
}//common::coreModule


#endif //COMMON_WINDOWSYSTEM_H
