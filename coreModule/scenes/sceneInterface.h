#ifndef GENERIC_SCENEINTERFACE_H
#define GENERIC_SCENEINTERFACE_H

#include "cocos2d.h"

namespace generic::coreModule {
    class sceneInterface : public cocos2d::Scene {
    public:
        virtual ~sceneInterface() = 0;

        virtual void onSceneLoading() = 0;
        virtual void onSceneClosing() = 0;
    };
}

#endif// GENERIC_SCENEINTERFACE_H
