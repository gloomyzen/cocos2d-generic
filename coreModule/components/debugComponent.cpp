#include "debugComponent.h"
#include "generic/debugModule/logManager.h"

using namespace generic::coreModule;

debugComponent::debugComponent() {
    _name = "debugComponent";
}

debugComponent::~debugComponent() {}

void debugComponent::update(float delta) {
    Component::update(delta);
}
