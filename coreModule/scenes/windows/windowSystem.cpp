#include "windowSystem.h"
#include "generic/coreModule/scenes/windows/windowBase.h"
#include "generic/utilityModule/logManager.h"
#include "generic/utilityModule/stringUtility.h"
#include "generic/coreModule/components/transformComponent.h"
#include <algorithm>

using namespace generic::coreModule;
using namespace ax;

windowSystem::windowSystem() {
    this->setName("windowViewer");
    this->setAnchorPoint({ 0.5f, 0.5f });
    auto transform = transformComponent::attachAndGetTransformComponent(this);
    transform->setStretch(1.f, 1.f);
    transform->setPivotPoint({ 0.5f, 0.5f });
}

void windowSystem::registerWindow(const std::string& name, const std::function<windowBase*()>& clb) {
    auto find = registeredWindowList.find(name);
    if (find == registeredWindowList.end()) {
        registeredWindowList[name] = clb;
    } else {
        LOG_ERROR(CSTRING_FORMAT("Trying to register a duplicate window '%s'!", name.c_str()));
    }
}

windowBase* windowSystem::requestWindow(const std::string& name, bool force) {
    auto find = registeredWindowList.find(name);
    if (find == registeredWindowList.end()) {
        return nullptr;
    }
    auto findOpened = std::find_if(openedWindowList.cbegin(), openedWindowList.cend(), [name](const std::pair<windowBase*, int>& c) {
        return c.first->getWindowName() == name;
    });
    auto findWaiting = std::find_if(waitingWindowList.cbegin(), waitingWindowList.cend(), [name](windowBase* node) {
        return node->getWindowName() == name;
    });
    auto window = registeredWindowList[name]();
    window->setWindowName(name);
    auto slotId = window->getEmitter()->onWindowClose.connect([this](const std::string& windowName){
        closeWindow(windowName);
    });
    if ((force && findOpened == openedWindowList.end()) || (openedWindowList.empty() && waitingWindowList.empty())) {
        openedWindowList.emplace_back(window, slotId);
        this->scheduleOnce(
          [this, window](float) {
              addChild(window);
              return true;
          },
          0.f,
          STRING_FORMAT("openWindow_%s", name.c_str()));
        return window;
    } else if (findWaiting == waitingWindowList.end() && findOpened == openedWindowList.end()) {
        waitingWindowList.push_back(window);
        return window;
    }
    return nullptr;
}

bool windowSystem::closeWindow(const std::string& name) {
    auto find = std::find_if(openedWindowList.begin(), openedWindowList.end(), [name](const std::pair<windowBase*, int>& c) {
        return c.first->getWindowName() == name;
    });
    auto taskName = STRING_FORMAT("closeWindowTask_%s_%u", name.c_str(), this->_ID);
    if (!this->isScheduled(taskName)) {
        this->scheduleOnce(
          [this, find, name](float) {
              if (find != openedWindowList.end()) {
                  (*find).first->removeFromParentAndCleanup(true);
                  (*find).first->getEmitter()->onWindowClose.disconnect((*find).second);
                  openedWindowList.erase(find);
              }
              auto taskName = STRING_FORMAT("openWindow_%s", name.c_str());
              if (!waitingWindowList.empty() && !this->isScheduled(taskName)) {
                  auto window = waitingWindowList.begin();
                  auto slotId = (*window)->getEmitter()->onWindowClose.connect([this](const std::string& windowName){
                      closeWindow(windowName);
                  });
                  openedWindowList.emplace_back((*window), slotId);
                  this->scheduleOnce(
                    [this, window](float) {
                        addChild(*window);
                        return true;
                    },
                    0.f,
                    taskName);
                  waitingWindowList.erase(window);
              }
              return true;
          },
          0.f,
          taskName);
    }
    return true;
}
