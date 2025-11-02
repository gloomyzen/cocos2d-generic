#pragma once
#ifndef GENERIC_DEBUGCOMPONENT_H
#define GENERIC_DEBUGCOMPONENT_H

#include "axmol/axmol.h"
#include <string_view>

namespace generic::coreModule {

    class debugComponent : public ax::Component {
    public:
        debugComponent();
        ~debugComponent() override;

        void update(float delta) override;

        bool isDebugEnabled();
        void setDebugEnabled(bool);

        void setLineColor(ax::Color32);
        ax::Color32 getLineColor();
        void setPointColor(ax::Color32);
        ax::Color32 getPointColor();

        static std::string_view DEBUG_COMPONENT_NAME;
    protected:
        bool _debugDrawEnabled = false;
        ax::Color32 _debugColorLine = ax::Color32::WHITE;
        ax::Color32 _debugColorPoint = ax::Color32::RED;

        ax::DrawNode* _debugDrawNode = nullptr;
    };

}// namespace generic::coreModule

#endif// GENERIC_DEBUGCOMPONENT_H
