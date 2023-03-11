#include "debugComponent.h"

using namespace generic::coreModule;
using namespace ax;

std::string_view debugComponent::DEBUG_COMPONENT_NAME = "debugComponent";

debugComponent::debugComponent() {
    _name = DEBUG_COMPONENT_NAME;
}

debugComponent::~debugComponent() {
    _debugDrawNode->removeFromParentAndCleanup(true);
}

void debugComponent::update(float delta) {
    Component::update(delta);
    if (isDebugEnabled() && _debugDrawNode) {
        _debugDrawNode->clear();
        auto anchor = getOwner()->getAnchorPoint();
        auto rect = getOwner()->getContentSize();
        auto pos = getOwner()->getPosition();

        Vec2 dot = { rect.width * anchor.x, rect.height * anchor.y };
        _debugDrawNode->drawPoint(dot, 4.f, _debugColorPoint);
        _debugDrawNode->drawRect(Vec2::ZERO, rect, _debugColorLine);
    }
}

bool debugComponent::isDebugEnabled() {
    return _debugDrawEnabled;
}

void debugComponent::setDebugEnabled(bool value) {
    _debugDrawEnabled = value;
    if (value && !_debugDrawNode) {
        _debugDrawNode = DrawNode::create();
        _debugDrawNode->setName("debugNode");
        getOwner()->addChild(_debugDrawNode);
    }
    if (value) {
        _debugDrawNode->setVisible(true);
    } else {
        _debugDrawNode->clear();
        _debugDrawNode->setVisible(false);
    }
}

void debugComponent::setLineColor(ax::Color4F color) {
    _debugColorLine = color;
}

void debugComponent::setPointColor(ax::Color4F color) {
    _debugColorPoint = color;
}

ax::Color4F debugComponent::getLineColor() {
    return _debugColorLine;
}

ax::Color4F debugComponent::getPointColor() {
    return _debugColorPoint;
}