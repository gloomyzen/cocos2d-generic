#ifndef MERCENARY_BATTLES_NODE3D_H
#define MERCENARY_BATTLES_NODE3D_H

#include "cocos2d.h"
#include <functional>
#include <utility>

namespace common::coreModule {
    class node3d : public cocos2d::Node {
      public:
        node3d() = default;
        ~node3d() override = default;
        CREATE_FUNC(node3d);

        void visit(Renderer* renderer, const Mat4& parentTransform, uint32_t parentFlags) override {
            // quick return if not visible. children won't be drawn.
            if (!_visible) {
                return;
            }

            uint32_t flags = processParentFlags(parentTransform, parentFlags);
//            flags |= FLAGS_RENDER_AS_3D;

            //
            Director* director = Director::getInstance();
            director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
            director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);

            bool visibleByCamera = isVisitableByVisitingCamera();

            int i = 0;

            if (!_children.empty()) {
                std::sort(std::begin(_children), std::end(_children), [](Node* n1, Node* n2) {
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

            director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
        }
    };
}// namespace common::coreModule
#endif// MERCENARY_BATTLES_NODE3D_H
