#ifndef GENERIC_SCENEINTERFACE_H
#define GENERIC_SCENEINTERFACE_H

#include "cocos2d.h"
#include "layersEnum.h"
#ifdef DEBUG
#include "generic/debugModule/imGuiLayer.h"
#endif

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
        void updateLayers(sceneInterface* prevScene) {
            // update imGuiLayer
#ifdef DEBUG
            if (!prevScene || !prevScene->imGuiLayer) {
                /// insert debug layer
                imGuiLayer = generic::debugModule::imGuiLayer::create();
                this->addChild(imGuiLayer, eGameLayers::DEBUG_LAYER);
            } else {
                prevScene->imGuiLayer->retain();
                prevScene->removeChild(prevScene->imGuiLayer, true);
                this->addChild(prevScene->imGuiLayer, eGameLayers::DEBUG_LAYER);
                imGuiLayer = prevScene->imGuiLayer;
                prevScene->imGuiLayer = nullptr;
            }
#endif
        }


    protected:
        bool physics = false;
        cocos2d::Vec2 defaultGravity = { 0.f, 0.f };
        cocos2d::LayerColor* bgLayer = nullptr;
#ifdef DEBUG
        generic::debugModule::imGuiLayer* imGuiLayer = nullptr;
#endif
    };
}// namespace generic::coreModule

#endif// GENERIC_SCENEINTERFACE_H
