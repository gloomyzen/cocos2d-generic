#include "pivotComponent.h"

using namespace common::coreModule;


pivotComponent::pivotComponent() {
	_name = "pivotComponent";
}

pivotComponent::~pivotComponent() {

}

void pivotComponent::update(float delta) {

	Component::update(delta);
}

void pivotComponent::setPivotPoint(cocos2d::Vec2 point) {
	pivotPoint = point;
}
