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
	//todo
//	1. create viewer with bg and prop
//	2. using windowholder at scene
//	3. create window factory
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
	if (force && find != registeredWindowList.end()) {
		auto window = registeredWindowList[name]();
		windowList.push_back(window);
		addChild(window);
		//request
		return true;
	}

	return false;
}
