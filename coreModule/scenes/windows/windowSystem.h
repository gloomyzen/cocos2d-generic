#ifndef GENERIC_WINDOWSYSTEM_H
#define GENERIC_WINDOWSYSTEM_H

#include "cocos2d.h"
#include "generic/coreModule/nodes/nodeProperties.h"
#include <map>
#include <string>
#include <vector>

namespace generic::coreModule {
    class windowBase;

    class windowSystem : public cocos2d::Node {
      public:
        windowSystem();
        ~windowSystem() = default;

        CREATE_FUNC(windowSystem);
        void registerWindow(const std::string&, const std::function<windowBase*()>&);
        windowBase* requestWindow(const std::string&, bool force = false);
        bool closeWindow(const std::string&);

      private:
        std::map<std::string, std::function<windowBase*()>> registeredWindowList;
        std::vector<windowBase*> openedWindowList;
        std::vector<windowBase*> waitingWindowList;
        std::string lastTask;
    };
}// namespace generic::coreModule


#endif// GENERIC_WINDOWSYSTEM_H
