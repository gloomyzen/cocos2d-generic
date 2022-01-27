#ifndef GENERIC_ARMATUREHOLDERNODE_H
#define GENERIC_ARMATUREHOLDERNODE_H

#include "cocos2d.h"
#if defined(CC_BUILD_WITH_DRANGBONES) && CC_BUILD_WITH_DRANGBONES
#include "DragonBones/CCDragonBonesHeaders.h"
#include "cocos-ext.h"
#include <functional>
#include <map>
#include <string>
#include <vector>

namespace generic::coreModule {

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

        void addChild(Node* child) override;
        void addChild(Node* child, int localZOrder) override;
        void addChild(Node* child, int localZOrder, int tag) override;
        void addChild(Node* child, int localZOrder, const std::string& name) override;

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
}// namespace generic::coreModule
#else
namespace generic::coreModule {

    class armatureNode : public cocos2d::Node {
    public:
        CREATE_FUNC(armatureNode);
    };
}// namespace generic::coreModule
#endif
#endif// GENERIC_ARMATUREHOLDERNODE_H
