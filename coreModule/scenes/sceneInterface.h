#ifndef GENERIC_SCENEINTERFACE_H
#define GENERIC_SCENEINTERFACE_H

#include "cocos2d.h"

namespace generic::coreModule {
    class sceneInterface : public cocos2d::Scene {
    public:
        virtual ~sceneInterface() {};

        virtual void onSceneLoading() {}
        virtual void onSceneClosing() {}

        void setPhysics(bool value) { physics = value; }
        bool isPhysics() { return physics; }
        void setGravity(cocos2d::Vec2 value) { defaultGravity = value; }
        cocos2d::Vec2 getGravity() { return defaultGravity; }


    protected:
        bool physics = false;
        cocos2d::Vec2 defaultGravity = {0.f, 0.f};
    };
}

#endif// GENERIC_SCENEINTERFACE_H
