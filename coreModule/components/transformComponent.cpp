#include "transformComponent.h"

using namespace generic::coreModule;
using namespace ax;

std::string_view transformComponent::TRANSFORM_COMPONENT_NAME = "transformComponent";

transformComponent::transformComponent() {
    _name = TRANSFORM_COMPONENT_NAME;
}

transformComponent::~transformComponent() {}

void transformComponent::update(float delta) {
    Component::update(delta);
    if (_isPivotPointEnabled) {
        if (_owner && _owner->getParent()) {
            auto parent = _owner->getParent();
            ax::Mat4 mat;
            mat.m[12] = parent->getContentSize().width * _pivotPoint.x;
            mat.m[13] = parent->getContentSize().height * _pivotPoint.y;
            _owner->setAdditionalTransform(&mat);
            if (_pivotPoint.x == 0.f && _pivotPoint.y == 0.f)
                _isPivotPointEnabled = false;
        }
    }
    if (_isStretchEnabled) {
        if (_owner && _owner->getParent()) {
            auto parent = _owner->getParent();
            auto size = parent->getContentSize();
            size.x *= _stretch.x;
            size.y *= _stretch.y;
            _owner->setContentSize(size);
        }
    }
}

transformComponent* transformComponent::attachAndGetTransformComponent(ax::Node* owner) {
    auto component = dynamic_cast<transformComponent*>(owner->getComponent(transformComponent::TRANSFORM_COMPONENT_NAME));
    if (!component) {
        component = new transformComponent();
        owner->addComponent(component);
    }
    return component;
}

ax::Vec2 transformComponent::getPivotPoint() const {
    return _pivotPoint;
}

void transformComponent::setPivotPoint(const ax::Vec2& _pivot) {
    _pivotPoint = _pivot;
    _isPivotPointEnabled = true;
}

void transformComponent::setStretch(const ax::Vec2& _s) {
    _stretch = _s;
    _isStretchEnabled = true;
}

void transformComponent::setStretch(float _x, float _y) {
    _stretch = { _x, _y };
    _isStretchEnabled = true;
}

void transformComponent::disableStretch() {
    _stretch = ax::Vec2::ZERO;
    _isStretchEnabled = false;
}

ax::Vec2 transformComponent::getStretch() const {
    return _stretch;
}