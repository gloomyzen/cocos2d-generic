#include "windowBase.h"

using namespace common::coreModule;
using namespace cocos2d;

windowBase::windowBase() {}

windowBase::~windowBase() {
	for (const auto& item : windowData) {
		delete item.second.get();
	}
}
