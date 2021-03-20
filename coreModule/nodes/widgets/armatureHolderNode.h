#ifndef COMMON_ARMATUREHOLDERNODE_H
#define COMMON_ARMATUREHOLDERNODE_H

#include "cocos2d.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"
#include <vector>
#include <string>
#include <functional>

namespace common {
    namespace coreModule {

        class armatureHolderNode : public cocos2d::Node {
          public:
            enum class eArmatureState {
                START = 0,
                LOOP_COMPLETE,
                COMPLETE,
                FADE_IN,
                FADE_IN_COMPLETE,
                FADE_OUT,
                FADE_OUT_COMPLETE,
                FRAME_EVENT,
                SOUND_EVENT,
            };

            CREATE_FUNC(armatureHolderNode);

            virtual void setDebug(bool value) override;
            virtual void addChild(Node * child) override;
            virtual void addChild(Node * child, int localZOrder) override;
            virtual void addChild(Node* child, int localZOrder, int tag) override;
            virtual void addChild(Node* child, int localZOrder, const std::string &name) override;

            dragonBones::CCArmatureDisplay* getArmatureNode();

            void setAnimation(const std::string& name, std::function<void(cocos2d::EventCustom*)> clb = nullptr);
            void setAnimationCallback(eArmatureState, std::function<void(cocos2d::EventCustom*)>);
            void removeAnimationCallback(eArmatureState);
            bool isEventsEnable() const;
            void setEventsEnabled(bool);

          private:
            const char* getEventType(eArmatureState);

          protected:
            bool handleEvents = false;
            dragonBones::CCArmatureDisplay* boneNode = nullptr;
        };
    }// namespace coreModule
}// namespace common

#endif// COMMON_ARMATUREHOLDERNODE_H
