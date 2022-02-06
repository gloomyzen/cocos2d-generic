#include "sceneInterface.h"
#include "generic/coreModule/physicsShape/physicsShapeCache.h"
#include "generic/audioModule/audioEngineInstance.h"

using namespace generic::coreModule;

void sceneInterface::onSceneLoading() {
    GET_PHYSICS_SHAPE().addShapesWithFile(getName());
    GET_AUDIO_ENGINE().onSceneLoading(getName());
}

void sceneInterface::onSceneClosing() {
    GET_PHYSICS_SHAPE().removeShapesWithFile(getName());
    GET_AUDIO_ENGINE().onSceneClosing(getName());
}
