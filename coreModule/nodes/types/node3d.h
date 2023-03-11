#pragma once
#ifndef GENERIC_NODE3D_H
#define GENERIC_NODE3D_H

#include "axmol.h"
#include <functional>
#include <utility>

namespace generic::coreModule {
    class static3dNode {
    public:
        void setStaticEnabled(bool value) {
            isEnabled = value;
        }
        bool isStaticEnabled() const {
            return isEnabled;
        }

    private:
        bool isEnabled = true;
    };

    class node3d : public ax::Node {
    public:
        node3d() = default;
        ~node3d() override = default;
        CREATE_FUNC(node3d);

        void visit(ax::Renderer* renderer, const ax::Mat4& parentTransform, uint32_t parentFlags) override {
            // quick return if not visible. children won't be drawn.
            if (!_visible) {
                return;
            }

            uint32_t flags = processParentFlags(parentTransform, parentFlags);

            ax::Director* director = ax::Director::getInstance();
            director->pushMatrix(ax::MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
            director->loadMatrix(ax::MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);

            bool visibleByCamera = isVisitableByVisitingCamera();

            int i = 0;

            if (!_children.empty()) {
                std::sort(std::begin(_children), std::end(_children), [](Node* n1, Node* n2) {
                    bool firstIsStatic = dynamic_cast<static3dNode*>(n1) && dynamic_cast<static3dNode*>(n1)->isStaticEnabled();
                    bool secondIsStatic = dynamic_cast<static3dNode*>(n2) && dynamic_cast<static3dNode*>(n2)->isStaticEnabled();
                    if (firstIsStatic && secondIsStatic) {
                        return (n1->getPositionY() > n2->getPositionY());
                    } else if (firstIsStatic && !secondIsStatic) {
                        return true;
                    } else if (!firstIsStatic && secondIsStatic) {
                        return false;
                    }
                    return (n1->getPositionY() > n2->getPositionY());
                });
                // draw children zOrder < 0
                for (auto size = _children.size(); i < size; i++) {
                    auto node = _children.at(i);

                    if (node)
                        node->visit(renderer, _modelViewTransform, flags);
                    else
                        break;
                }
                // self draw
                if (visibleByCamera)
                    this->draw(renderer, _modelViewTransform, flags);

                for (auto it = _children.cbegin() + i, itCend = _children.cend(); it != itCend; ++it)
                    (*it)->visit(renderer, _modelViewTransform, flags);
            } else if (visibleByCamera) {
                this->draw(renderer, _modelViewTransform, flags);
            }

            director->popMatrix(ax::MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
        }
    };
}// namespace generic::coreModule
#endif// GENERIC_NODE3D_H
