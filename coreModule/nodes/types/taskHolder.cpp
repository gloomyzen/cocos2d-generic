#include "taskHolder.h"
#include "axmol.h"
#include "generic/coreModule/components/taskHolderComponent.h"

using namespace generic::coreModule;

taskHolder::taskHolder() {
    if (auto node = dynamic_cast<ax::Node*>(this)) {
        node->addComponent(new taskHolderComponent());
    }
}

void taskHolder::executeTasks() {
    if (_allTasksLoaded)
        return;
    _allTasksLoaded = true;

    _allTasks = getTasks();

    proceed();
}

void taskHolder::proceed() {
    if (_allTasks.empty())
        return;
    auto task = std::move(_allTasks.front());
    _allTasks.erase(_allTasks.begin());

    if (auto currentNode = dynamic_cast<ax::Node*>(this)) {
        currentNode->scheduleOnce(
          [this, task](float) {
              if (task) {
                  auto res = task();
                  switch (res) {
                  case eTasksStatus::STATUS_OK:
                      proceed();
                      break;
                  case eTasksStatus::STATUS_ERROR_BREAK:
                      return false;
                      break;
                  }
              }
              return true;
          },
          0.f,
          "runTasks");
    }
}