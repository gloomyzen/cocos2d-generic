#include "sceneInterface.h"
#include "generic/coreModule/physicsShape/physicsShapeCache.h"

using namespace generic::coreModule;

void sceneInterface::onSceneLoading() {
    GET_PHYSICS_SHAPE().addShapesWithFile(getName());
}

void sceneInterface::onSceneClosing() {
    GET_PHYSICS_SHAPE().removeShapesWithFile(getName());
}
