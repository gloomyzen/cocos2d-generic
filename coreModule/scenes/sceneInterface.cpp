#include "sceneInterface.h"
#include "generic/coreModule/physicsShape/physicsShapeCache.h"
#include "generic/audioModule/audioEngineInstance.h"
#include "generic/coreModule/nodes/types/taskHolder.h"

using namespace generic::coreModule;

sceneInterface::sceneInterface() {
    initLayerColor(ax::Color3B::BLACK);
}

void sceneInterface::onSceneLoading() {
    GET_PHYSICS_SHAPE().addShapesWithFile(getName().data());
    GET_AUDIO_ENGINE().onSceneLoading(getName().data());
}

void sceneInterface::onSceneClosing() {
    GET_PHYSICS_SHAPE().removeShapesWithFile(getName().data());
    GET_AUDIO_ENGINE().onSceneClosing(getName().data());
}

void sceneInterface::onEnter() {
    ax::Scene::onEnter();
    if (auto item = dynamic_cast<generic::coreModule::taskHolder*>(this)) {
        auto atp = ax::AsyncTaskPool::getInstance();
        atp->enqueue(ax::AsyncTaskPool::TaskType::TASK_OTHER, [](void*){}, nullptr,
          [item]() {
              item->executeTasks();
          });
    }
}
