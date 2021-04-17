#include "windowBase.h"
#include "common/utilityModule/stringUtility.h"

#include <utility>

using namespace common::coreModule;
using namespace cocos2d;

windowBase::windowBase() { initWindow(); }
windowBase::windowBase(std::string name) : windowName(std::move(name)) { initWindow(); }

windowBase::~windowBase() {
    for (auto& item : windowData) { delete item.second; }
    windowData.clear();
}

void windowBase::initWindow() {
    this->setName("windowBase");
    loadProperty(STRING_FORMAT("windows/%s", this->getName().c_str()), dynamic_cast<Node*>(this));
    setOnTouchEnded([this]() {
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
    auto clb = cocos2d::CallFunc::create([this]() {
        currentState = eWindowState::OPENED;
        setCascadeOpacityEnabled(false);
    });
    auto seq = Sequence::create(spawn, clb, nullptr);
    runAction(seq);
}

void windowBase::closeWindow() {
    auto closeClb = getCallback("safeClose");
    currentState = eWindowState::CLOSING;
    setCascadeOpacityEnabled(true);
    auto fadeTo = FadeTo::create(0.08f, 0.0f);
    auto scaleBy = ScaleBy::create(0.12f, 0.7f);
    auto spawn = Spawn::createWithTwoActions(scaleBy, fadeTo);
    auto clb = cocos2d::CallFunc::create([closeClb]() {
        if (closeClb)
            closeClb();
    });
    auto seq = Sequence::create(spawn, clb, nullptr);
    runAction(seq);
}
