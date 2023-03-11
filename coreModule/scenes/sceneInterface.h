#pragma once
#ifndef GENERIC_SCENEINTERFACE_H
#define GENERIC_SCENEINTERFACE_H

#include "axmol.h"
#include "layersEnum.h"
#include "generic/coreModule/scenes/windows/windowSystem.h"
#include <vector>
#include <tuple>

namespace generic::coreModule {
    class sceneInterface : public ax::Scene {
    public:
        sceneInterface();

        virtual ~sceneInterface() {}

        virtual void onSceneLoading();
        virtual void onSceneClosing();

        void onEnter() override;

        void setPhysics(bool value) {
            _physics = value;
        }
        bool isPhysics() const {
            return _physics;
        }
        void setGravity(ax::Vec2 value) {
            _defaultGravity = value;
        }
        ax::Vec2 getGravity() {
            return _defaultGravity;
        }
        void initLayerColor(ax::Color3B color) {
            if (!_bgLayer) {
                _bgLayer = new ax::LayerColor();
                _bgLayer->setContentSize(ax::Director::getInstance()->getVisibleSize());
                this->addChild(_bgLayer, static_cast<int>(eGameLayers::BACKGROUND));
            }
            _bgLayer->setColor(color);
        }
        void updateLayers(sceneInterface* prevScene) {
            _windowViewer = new windowSystem();
            this->addChild(_windowViewer, eGameLayers::WINDOW);
        }
        void setFadeTransition(float value) {
            _fadeTransitionTime = value;
        }
        float getFadeTransition() const {
            return _fadeTransitionTime;
        }
        windowSystem* getWindowSystem() { return _windowViewer; }
        void setValues(const ax::ValueMap& value) {
            _sceneData = value; }

    protected:
        bool _physics = false;
        ax::Vec2 _defaultGravity = { 0.f, 0.f };
        ax::LayerColor* _bgLayer = nullptr;
        windowSystem* _windowViewer = nullptr;
        float _fadeTransitionTime = .4f;
        ax::ValueMap _sceneData;
    };
}// namespace generic::coreModule

#endif// GENERIC_SCENEINTERFACE_H
