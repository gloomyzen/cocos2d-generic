#ifndef GENERIC_SCENEINTERFACE_H
#define GENERIC_SCENEINTERFACE_H

#include "cocos2d.h"
#include "layersEnum.h"

namespace generic::coreModule {
    class sceneInterface : public cocos2d::Scene {
    public:
        sceneInterface() {
            initLayerColor(cocos2d::Color3B::BLACK);
        };

        virtual ~sceneInterface(){};

        virtual void onSceneLoading() {}
        virtual void onSceneClosing() {}

        void setPhysics(bool value) {
            physics = value;
        }
        bool isPhysics() {
            return physics;
        }
        void setGravity(cocos2d::Vec2 value) {
            defaultGravity = value;
        }
        cocos2d::Vec2 getGravity() {
            return defaultGravity;
        }
        void initLayerColor(cocos2d::Color3B color) {
            if (!bgLayer) {
                bgLayer = new cocos2d::LayerColor();
                bgLayer->setContentSize(cocos2d::Director::getInstance()->getVisibleSize());
                this->addChild(bgLayer, static_cast<int>(eGameLayers::BACKGROUND));
            }
            bgLayer->setColor(color);
        }


    protected:
        bool physics = false;
        cocos2d::Vec2 defaultGravity = { 0.f, 0.f };
        cocos2d::LayerColor* bgLayer = nullptr;
    };
}// namespace generic::coreModule

#endif// GENERIC_SCENEINTERFACE_H
