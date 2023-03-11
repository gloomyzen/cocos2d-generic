#include "taskHolderComponent.h"
#include "generic/coreModule/nodes/types/taskHolder.h"

using namespace generic::coreModule;
using namespace ax;

std::string_view taskHolderComponent::TASK_HOLDER_COMPONENT_NAME = "taskHolderComponent";

taskHolderComponent::taskHolderComponent() {
    _name = TASK_HOLDER_COMPONENT_NAME;
}

void taskHolderComponent::onEnter() {
    Component::onEnter();
    if (_taskHolderPtr && _taskHolderPtr->getTasksLoad() == eTasksLoad::ON_ENTER) {
        _taskHolderPtr->executeTasks();
    }
}

void taskHolderComponent::setOwner(ax::Node* owner) {
    Component::setOwner(owner);
    if (auto node = dynamic_cast<taskHolder*>(_owner)) {
        _taskHolderPtr = node;
    }
}