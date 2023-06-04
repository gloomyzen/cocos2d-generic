#pragma once
#ifndef GENERIC_TRANSFORMCOMPONENT_H
#define GENERIC_TRANSFORMCOMPONENT_H

#include "axmol.h"
#include <string_view>

namespace generic::coreModule {

    class transformComponent : public ax::Component {
    public:
        transformComponent();
        ~transformComponent() override;

        void update(float delta) override;
        ax::Vec2 getPivotPoint() const;
        void setPivotPoint(const ax::Vec2& _pivot);
        void setStretch(const ax::Vec2& _s);
        void setStretch(float _x, float _y);
        void disableStretch();
        ax::Vec2 getStretch() const;
        static transformComponent* attachAndGetTransformComponent(ax::Node* owner);
        static std::string_view TRANSFORM_COMPONENT_NAME;

    protected:
        bool _isPivotPointEnabled = false;
        bool _isStretchEnabled = false;
        ax::Vec2 _pivotPoint = ax::Vec2::ZERO;
        ax::Vec2 _stretch = ax::Vec2::ZERO;
    };

}// namespace generic::coreModule


#endif// GENERIC_TRANSFORMCOMPONENT_H
