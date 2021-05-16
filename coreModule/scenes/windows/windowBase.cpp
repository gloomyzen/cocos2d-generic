#include "windowBase.h"
#include "common/utilityModule/stringUtility.h"
#include "common/debugModule/logManager.h"
#include "common/coreModule/scenes/windows/windowSystem.h"

#include <utility>

using namespace common::coreModule;
using namespace cocos2d;

windowBase::windowBase() { initWindow(); }
windowBase::windowBase(std::string name) : windowName(std::move(name)) { initWindow(); }

windowBase::~windowBase() {
    this->unscheduleAllCallbacks();
    for (auto& item : windowData) { delete item.second; }
    windowData.clear();
}

void windowBase::initWindow() {
    this->setName("windowBase");
    loadProperty(STRING_FORMAT("windows/%s", this->getName().c_str()), dynamic_cast<Node*>(this));
    if (auto bgNode = dynamic_cast<cocos2d::Sprite*>(findNode("buttonNode"))) {
        setButtonBgSprite(bgNode);
    }
    setOnTouchEnded([this]() {
        if (handleMissClick) {
            closeWindow();
        }
    });

    currentState = eWindowState::OPENING;
    setScale(0.8f);
    setOpacity(static_cast<uint8_t>(20));
    setCascadeOpacityEnabled(true);
    auto fadeTo = FadeTo::create(0.08f, 255.0f);
    auto scaleBy = ScaleBy::create(0.08f, 1.25f);
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
    auto clb = cocos2d::CallFunc::create([this, closeClb]() {
           if (closeClb)
               closeClb();
           GET_GAME_MANAGER().getWindowSystem()->closeWindow(getWindowName());
    });
    setCascadeOpacityEnabled(true);
    switch (closeAnim) {
    case eWindowAnim::DEFAULT: {
        auto fadeTo = FadeTo::create(0.08f, 0.0f);
        auto scaleBy = ScaleBy::create(0.08f, 0.8f);
        auto spawn = Spawn::createWithTwoActions(scaleBy, fadeTo);
        auto seq = Sequence::create(spawn, clb, nullptr);
        runAction(seq);
    }
        break;
    case eWindowAnim::SCALE: {
        auto scaleBy = ScaleBy::create(0.08f, 0.8f);
        auto seq = Sequence::create(scaleBy, clb, nullptr);
        runAction(seq);
    }
        break;
    case eWindowAnim::FADE: {
        auto fadeTo = FadeTo::create(0.08f, 0.0f);
        auto seq = Sequence::create(fadeTo, clb, nullptr);
        runAction(seq);
    }
        break;
    }
}
