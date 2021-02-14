#include "windowBase.h"

using namespace common::coreModule;
using namespace cocos2d;

windowBase::windowBase() {}
windowBase::windowBase(const std::string& name) : windowName(name) {}

windowBase::~windowBase() {
	for (const auto& item : windowData) {
		delete item.second.get();
	}
}
