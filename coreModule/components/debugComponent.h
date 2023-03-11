#pragma once
#ifndef GENERIC_DEBUGCOMPONENT_H
#define GENERIC_DEBUGCOMPONENT_H

#include "axmol.h"
#include <string_view>

namespace generic::coreModule {

    class debugComponent : public ax::Component {
    public:
        debugComponent();
        ~debugComponent() override;

        void update(float delta) override;

        bool isDebugEnabled();
        void setDebugEnabled(bool);

        void setLineColor(ax::Color4F);
        ax::Color4F getLineColor();
        void setPointColor(ax::Color4F);
        ax::Color4F getPointColor();

        static std::string_view DEBUG_COMPONENT_NAME;
    protected:
        bool _debugDrawEnabled = false;
        ax::Color4F _debugColorLine = ax::Color4F::WHITE;
        ax::Color4F _debugColorPoint = ax::Color4F::RED;

        ax::DrawNode* _debugDrawNode = nullptr;
    };

}// namespace generic::coreModule

#endif// GENERIC_DEBUGCOMPONENT_H
