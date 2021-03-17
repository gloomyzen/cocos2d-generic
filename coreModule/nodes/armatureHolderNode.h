#ifndef COMMON_ARMATUREHOLDERNODE_H
#define COMMON_ARMATUREHOLDERNODE_H

#include "cocos2d.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"
#include <vector>

namespace common {
    namespace coreModule {

        class armatureHolderNode : public cocos2d::Node {
          public:
            CREATE_FUNC(armatureHolderNode);

            virtual void setDebug(bool value) override {
                Node::setDebug(value);
                for (auto item : getChildren()) {
                    item->setDebug(value);
                }
            }

            dragonBones::CCArmatureDisplay* getArmatureNode() {
                for (auto item : getChildren()) {
                    if (auto armature = dynamic_cast<dragonBones::CCArmatureDisplay*>(item)) {
                        return armature;
                    }
                }
                return nullptr;
            }
        };
    }// namespace coreModule
}// namespace common

#endif// COMMON_ARMATUREHOLDERNODE_H
