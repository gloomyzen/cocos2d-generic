#include "windowBase.h"

#include <utility>

using namespace common::coreModule;
using namespace cocos2d;

windowBase::windowBase() {
	initWindow();
}
windowBase::windowBase(std::string name) : windowName(std::move(name)) {
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

	currentState = eWindowState::OPENING;
	setScale(0.5f);
	setOpacity(static_cast<uint8_t>(20));
	setCascadeOpacityEnabled(true);
	auto fadeTo = FadeTo::create(0.12f, 255.0f);
	auto scaleBy = ScaleBy::create(0.12f, 2.0f);
	auto spawn = Spawn::createWithTwoActions(scaleBy, fadeTo);
	auto clb = cocos2d::CallFunc::create([this](){
		currentState = eWindowState::OPENED;
		setCascadeOpacityEnabled(false);
	});
	auto seq = Sequence::create(spawn, clb, nullptr);
	runAction(seq);
}

void windowBase::closeWindow() {
	auto closeClb = getData<std::function<void()>>("safeClose", [](){});
	currentState = eWindowState::CLOSING;
	setCascadeOpacityEnabled(true);
	auto fadeTo = FadeTo::create(0.08f, 0.0f);
	auto scaleBy = ScaleBy::create(0.12f, 0.5f);
	auto spawn = Spawn::createWithTwoActions(scaleBy, fadeTo);
	auto clb = cocos2d::CallFunc::create([closeClb](){
		if (closeClb)
			closeClb();
	});
	auto seq = Sequence::create(spawn, clb, nullptr);
	runAction(seq);
}
