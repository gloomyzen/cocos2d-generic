#pragma once
#ifndef GENERIC_DRAWNODEBASE_H
#define GENERIC_DRAWNODEBASE_H

#include "axmol.h"
#include <functional>
#include <utility>

namespace generic::coreModule {
    class drawNodeBase : public ax::DrawNode {
    private:
        bool autoUpdateEnabled = false;
        ax::Color4F drawColor;
    public:
        drawNodeBase() {
            setName("drawNodeBase");
        };
        ~drawNodeBase() override = default;
        CREATE_FUNC(drawNodeBase);

        bool getAutoUpdateEnabled() const { return autoUpdateEnabled;}
        void setAutoUpdateEnabled(bool value) {
            autoUpdateEnabled = value;
        }

        void setDrawColor(ax::Color4F value) {
            drawColor = value;
        }

        void forceUpdateRect() {
            clear();
            ax::Size rect;
            if (_parent) {
                rect = _parent->getContentSize();
                setContentSize(rect);
            }
            drawSolidRect(ax::Vec2::ZERO, rect, drawColor);
        }

        void visit(ax::Renderer* renderer, const ax::Mat4 &parentTransform, uint32_t parentFlags) override {
            if (autoUpdateEnabled && _parent) {
                auto first = getContentSize();
                auto second = _parent->getContentSize();
                if (first.width != second.width || first.height != second.height ) {
                    forceUpdateRect();
                }
            }
            DrawNode::visit(renderer, parentTransform, parentFlags);
        }

    };
}

#endif// GENERIC_DRAWNODEBASE_H
