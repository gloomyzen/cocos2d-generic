#include "windowBase.h"
#include "generic/coreModule/scenes/windows/windowSystem.h"
#include "generic/utilityModule/logManager.h"
#include "generic/utilityModule/stringUtility.h"
#include "generic/utilityModule/findUtility.h"
#include <fmt/core.h>

#include <utility>

using namespace generic::coreModule;
using namespace ax;

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
    initWithProperties(fmt::format("windows/{}", this->getName().data()));
    removeJsonData();
    if (auto bgNode = dynamic_cast<ax::Sprite*>(generic::utilityModule::findNode(this, "buttonNode"))) {
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
    auto clb = ax::CallFunc::create([this]() {
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
    auto clb = ax::CallFunc::create([this, closeClb]() {
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
