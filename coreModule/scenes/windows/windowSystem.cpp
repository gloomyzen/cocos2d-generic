#include "windowSystem.h"

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
