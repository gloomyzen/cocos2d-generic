#include "sceneInterface.h"
#include "generic/audioModule/audioEngineInstance.h"
#include "generic/coreModule/nodes/types/taskHolder.h"

using namespace generic::coreModule;

sceneInterface::sceneInterface() {
    initLayerColor(ax::Color32::BLACK);
}

void sceneInterface::onSceneLoading() {
    GET_AUDIO_ENGINE().onSceneLoading(getName().data());
}

void sceneInterface::onSceneClosing() {
    GET_AUDIO_ENGINE().onSceneClosing(getName().data());
}

void sceneInterface::onEnter() {
    ax::Scene::onEnter();
    if (auto item = dynamic_cast<generic::coreModule::taskHolder*>(this)) {
        auto atp = ax::Director::getInstance()->getJobSystem();
        atp->enqueue([item]() {
            item->executeTasks();
        });
    }
}
