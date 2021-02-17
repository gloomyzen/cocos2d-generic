#include "windowBase.h"

#include <utility>

using namespace common::coreModule;
using namespace cocos2d;

windowBase::windowBase() {
	initWindow();
}
windowBase::windowBase(std::string  name) : windowName(std::move(name)) {
	initWindow();
}

windowBase::~windowBase() {
	for (auto& item : windowData) {
		delete item.second;
		item.second = nullptr;
	}
}

void windowBase::initWindow() {
	this->setName("windowBase");
	loadProperty(STRING_FORMAT("windows/%s", this->getName().c_str()), dynamic_cast<Node*>(this));
	setOnTouchEnded([this](cocos2d::Touch* touch, cocos2d::Event* event) {
		if (handleMissClick) {
			closeWindow();
		}
	});
}

void windowBase::closeWindow() {
	auto clb = getData<std::function<void()>>("safeClose", [](){});
	if (clb)
		clb();
}
