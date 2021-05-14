#include "windowSystem.h"
#include "common/coreModule/scenes/windows/windowBase.h"
#include "common/debugModule/logManager.h"
#include "common/utilityModule/stringUtility.h"

using namespace common::coreModule;
using namespace cocos2d;

windowSystem::windowSystem() {
    this->setName("windowViewer");
    this->setStretch(1.f, 1.f);
    this->setAnchorPoint({ 0.5f, 0.5f });
    this->setPivotPoint({ 0.5f, 0.5f });
}

void windowSystem::registerWindow(const std::string& name, const std::function<windowBase*()>& clb) {
    auto find = registeredWindowList.find(name);
    if (find == registeredWindowList.end()) {
        registeredWindowList[name] = clb;
    } else {
        LOG_ERROR(
            STRING_FORMAT("windowSystem::registerWindow: trying to register a duplicate window '%s'!", name.c_str()));
    }
}

windowBase* windowSystem::requestWindow(const std::string& name, bool force) {
    auto find = registeredWindowList.find(name);
    if (find == registeredWindowList.end()) {
        return nullptr;
    }
    auto findOpened = std::find_if(openedWindowList.cbegin(), openedWindowList.cend(), [name](windowBase* node) {
        return node->getWindowName() == name;
    });
    auto findWaiting = std::find_if(waitingWindowList.cbegin(), waitingWindowList.cend(), [name](windowBase* node) {
        return node->getWindowName() == name;
    });
    auto window = registeredWindowList[name]();
    window->setWindowName(name);
    auto taskProcess = this->isScheduled(STRING_FORMAT("closeWindowTask_%s_%u", name.c_str(), this->_ID));
    if ((force && findOpened == openedWindowList.end() && !taskProcess) || (openedWindowList.empty() && waitingWindowList.empty())) {
        openedWindowList.push_back(window);
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
    auto find = std::find_if(openedWindowList.begin(), openedWindowList.end(), [name](windowBase* node) {
        return node->getWindowName() == name;
    });
    auto taskName = STRING_FORMAT("closeWindowTask_%s_%u", name.c_str(), this->_ID);
    if (!this->isScheduled(taskName)) {
        this->scheduleOnce(
            [this, find, name](float) {
                   if (find != openedWindowList.end()) {
                       (*find)->removeFromParentAndCleanup(true);
                       openedWindowList.erase(find);
                   }
                   auto taskName = STRING_FORMAT("openWindow_%s", name.c_str());
                   if (!waitingWindowList.empty() && !this->isScheduled(taskName)) {
                       auto window = waitingWindowList.begin();
                       openedWindowList.push_back(*window);
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
