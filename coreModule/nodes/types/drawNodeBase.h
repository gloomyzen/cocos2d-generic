#ifndef GENERIC_DRAWNODEBASE_H
#define GENERIC_DRAWNODEBASE_H

#include "cocos2d.h"
#include <functional>
#include <utility>

namespace generic::coreModule {
    class drawNodeBase : public cocos2d::DrawNode {
    private:
        bool autoUpdateEnabled = false;
        cocos2d::Color4F drawColor;
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

        void setDrawColor(cocos2d::Color4F value) {
            drawColor = value;
        }

        void forceUpdateRect() {
            clear();
            cocos2d::Size rect;
            if (_parent) {
                rect = _parent->getContentSize();
                setContentSize(rect);
            }
            drawSolidRect(cocos2d::Vec2::ZERO, rect, drawColor);
        }

        void visit(cocos2d::Renderer* renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags) override {
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
