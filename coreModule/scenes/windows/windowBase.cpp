#include "windowBase.h"
#include "generic/coreModule/scenes/windows/windowSystem.h"
#include "generic/debugModule/logManager.h"
#include "generic/utilityModule/stringUtility.h"

#include <utility>

using namespace generic::coreModule;
using namespace cocos2d;

windowBase::windowBase() {
    initWindow();
}
windowBase::windowBase(std::string name) : windowName(std::move(name)) {
    initWindow();
}

windowBase::~windowBase() {
    this->unscheduleAllCallbacks();
    for (auto& item : windowData) {
        delete item.second;
    }
    windowData.clear();
}

void windowBase::initWindow() {
    this->setName("windowBase");
    initWithProperties(STRING_FORMAT("windows/%s", this->getName().c_str()));
    removeJsonData();
    if (auto bgNode = dynamic_cast<cocos2d::Sprite*>(findNode("buttonNode"))) {
        //todo fix this, buttonNode type was deleted
//        setButtonBgSprite(bgNode);
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
        getEmitter()->onWindowOpen.emit();
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
        getEmitter()->onWindowClose.emit(getWindowName());
    });
    setCascadeOpacityEnabled(true);
    switch (closeAnim) {
    case eWindowAnim::DEFAULT: {
        auto fadeTo = FadeTo::create(0.08f, 0.0f);
        auto scaleBy = ScaleBy::create(0.08f, 0.8f);
        auto spawn = Spawn::createWithTwoActions(scaleBy, fadeTo);
        auto seq = Sequence::create(spawn, clb, nullptr);
        runAction(seq);
    } break;
    case eWindowAnim::SCALE: {
        auto scaleBy = ScaleBy::create(0.08f, 0.8f);
        auto seq = Sequence::create(scaleBy, clb, nullptr);
        runAction(seq);
    } break;
    case eWindowAnim::FADE: {
        auto fadeTo = FadeTo::create(0.08f, 0.0f);
        auto seq = Sequence::create(fadeTo, clb, nullptr);
        runAction(seq);
    } break;
    }
}
