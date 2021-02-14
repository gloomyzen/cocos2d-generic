#include "windowSystem.h"
#include "common/debugModule/logManager.h"
#include "common/coreModule/scenes/windows/windowBase.h"

using namespace common::coreModule;
using namespace cocos2d;

windowSystem::windowSystem() {
	this->setName("windowViewer");
	this->setStretch(1.f, 1.f);
	this->setAnchorPoint({0.5f, 0.5f});
	this->setPivotPoint({0.5f, 0.5f});
//	todo
//	 1 opening animation% opening, close
}

void windowSystem::registerWindow(const std::string& name, const std::function<windowBase *()>& clb) {
	auto find = registeredWindowList.find(name);
	if (find == registeredWindowList.end()) {
		registeredWindowList[name] = clb;
	} else {
		LOG_ERROR(STRING_FORMAT("windowSystem::registerWindow: trying to register a duplicate window '%s'!", name.c_str()));
	}
}

bool windowSystem::requestWindow(const std::string& name, bool force) {
	auto find = registeredWindowList.find(name);
	if (find == registeredWindowList.end()) {
		return false;
	}
	auto findOpened = std::find_if(openedWindowList.cbegin(), openedWindowList.cend(), [name](windowBase* node){
		return node->getWindowName() == name;
	});
	auto findWaiting = std::find_if(waitingWindowList.cbegin(), waitingWindowList.cend(), [name](windowBase* node){
		return node->getWindowName() == name;
	});
	std::function clb = [this, name](){
		closeWindow(name);
	};
	auto window = registeredWindowList[name]();
	window->setData<std::function<void()>>("safeClose", clb);
	window->setWindowName(name);
	if (force || (openedWindowList.empty() && waitingWindowList.empty())) {
		openedWindowList.push_back(window);
		addChild(window);
		return true;
	} else if (findWaiting == waitingWindowList.end() && findOpened == openedWindowList.end()) {
		waitingWindowList.push_back(window);
		return true;
	}
	return false;
}

bool windowSystem::closeWindow(const std::string& name) {
	auto find = std::find_if(openedWindowList.cbegin(), openedWindowList.cend(), [name](windowBase* node){
		return node->getWindowName() == name;
	});
	if (find != openedWindowList.end()) {
		(*find)->removeFromParent();
		delete *find;
		openedWindowList.erase(find);
	}
	if (!waitingWindowList.empty()) {
		auto window = waitingWindowList.begin();
		openedWindowList.push_back(*window);
		addChild(*window);
		waitingWindowList.erase(window);
	}
	return false;
}
