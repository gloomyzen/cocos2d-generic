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
    //todo
}

transformComponent* transformComponent::attachAndGetTransformComponent(ax::Node* owner) {
    auto component = dynamic_cast<transformComponent*>(owner->getComponent(transformComponent::TRANSFORM_COMPONENT_NAME));
    if (!component) {
        component = new transformComponent();
        owner->addComponent(component);
    }
    return component;
}
