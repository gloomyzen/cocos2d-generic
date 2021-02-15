#include "windowBase.h"

using namespace common::coreModule;
using namespace cocos2d;

windowBase::windowBase() {
	initWindow();
}
windowBase::windowBase(const std::string& name) : windowName(name) {
	initWindow();
}

windowBase::~windowBase() {
	for (const auto& item : windowData) {
		delete item.second.get();
	}
}

void windowBase::initWindow() {
	this->setName("windowBase");
	loadProperty(STRING_FORMAT("windows/%s", this->getName().c_str()), dynamic_cast<Node*>(this));
}
