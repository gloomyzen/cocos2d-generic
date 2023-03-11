#pragma once
#ifndef GENERIC_TASKHOLDERCOMPONENT_H
#define GENERIC_TASKHOLDERCOMPONENT_H

#include "axmol.h"
#include <string_view>

namespace generic::coreModule {

    //forward
    class taskHolder;

    class taskHolderComponent : public ax::Component {
    public:
        taskHolderComponent();

        void onEnter() override;
        void setOwner(ax::Node* owner) override;

        static std::string_view TASK_HOLDER_COMPONENT_NAME;
    private:
        taskHolder* _taskHolderPtr = nullptr;
    };

}// namespace generic::coreModule


#endif// GENERIC_TASKHOLDERCOMPONENT_H
