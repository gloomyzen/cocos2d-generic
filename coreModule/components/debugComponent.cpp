#include "debugComponent.h"

using namespace generic::coreModule;
using namespace cocos2d;

debugComponent::debugComponent() {
    _name = "debugComponent";
}

debugComponent::~debugComponent() {
    _debugDrawNode->removeFromParentAndCleanup(true);
}

void debugComponent::update(float delta) {
    Component::update(delta);
    if (isDebug() && _debugDrawNode) {
        _debugDrawNode->clear();
        auto anchor = getOwner()->getAnchorPoint();
        auto rect = getOwner()->getContentSize();
        auto pos = getOwner()->getPosition();

        Vec2 dot = { rect.width * anchor.x, rect.height * anchor.y };
        _debugDrawNode->drawPoint(dot, 4.f, _debugColorPoint);
        _debugDrawNode->drawRect(Vec2::ZERO, rect, _debugColorLine);
    }
}

bool debugComponent::isDebug() const {
    return _debugDrawEnabled;
}

void debugComponent::setDebug(bool value) {
    _debugDrawEnabled = value;
    if (value && !_debugDrawNode) {
        _debugDrawNode = DrawNode::create();
        _debugDrawNode->setName("debugNode");
        getOwner()->addChild(_debugDrawNode);
    }
    if (value) {
        _debugDrawNode->setVisible(true);
    }
    if (!value) {
        _debugDrawNode->clear();
        _debugDrawNode->setVisible(false);
    }
}

void debugComponent::setLineColor(cocos2d::Color4F color) {
    _debugColorLine = color;
}

void debugComponent::setPointColor(cocos2d::Color4F color) {
    _debugColorPoint = color;
}
