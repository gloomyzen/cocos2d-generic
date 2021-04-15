#ifndef COMMON_ARMATUREHOLDERNODE_H
#define COMMON_ARMATUREHOLDERNODE_H

#include "cocos2d.h"
#include "DragonBones/CCDragonBonesHeaders.h"
#include "cocos-ext.h"
#include <vector>
#include <string>
#include <functional>
#include <map>

namespace common {
    namespace coreModule {

        class armatureNode : public cocos2d::Node {
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

            CREATE_FUNC(armatureNode);

//            void setDebug(bool value) override;
            void addChild(Node * child) override;
            void addChild(Node * child, int localZOrder) override;
            void addChild(Node* child, int localZOrder, int tag) override;
            void addChild(Node* child, int localZOrder, const std::string &name) override;

            dragonBones::CCArmatureDisplay* getArmatureNode();

            void setAnimation(const std::string& name, std::function<void(cocos2d::EventCustom*)> clb = nullptr);
            void setAnimationCallback(eArmatureState, std::function<void(cocos2d::EventCustom*)>);
            void removeAnimationCallback(eArmatureState);
            void setCustomAnimationCallback(const std::string&, const std::function<void(cocos2d::EventCustom*)>&);
            void removeCustomAnimationCallback(const std::string&);
            bool isEventsEnable() const;
            void setEventsEnabled(bool);

          private:
            const char* getEventType(eArmatureState);

          protected:
            bool handleEvents = false;
            dragonBones::CCArmatureDisplay* boneNode = nullptr;
            std::map<std::string, std::function<void(cocos2d::EventCustom*)>> customCallbacksMap;
        };
    }// namespace coreModule
}// namespace common

#endif// COMMON_ARMATUREHOLDERNODE_H
