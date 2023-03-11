#pragma once
#ifndef GENERIC_SHADERSNODE_H
#define GENERIC_SHADERSNODE_H

#include "axmol.h"
#include "renderer/CCRenderState.h"
#ifdef ADXE_VERSION
#include "spine/SkeletonTwoColorBatch.h"
#endif
#include "spine/spine-cocos2dx.h"
#include <functional>
#include <utility>

#define SET_UNIFORM(ps, name, value)                 \
    do {                                             \
        decltype(value) __v = value;                 \
        auto __loc = (ps)->getUniformLocation(name); \
        (ps)->setUniform(__loc, &__v, sizeof(__v));  \
    } while (false)

#define SET_TEXTURE(ps, name, idx, value)            \
    do {                                             \
        auto* __v = value;                           \
        auto __loc = (ps)->getUniformLocation(name); \
        (ps)->setTexture(__loc, idx, __v);           \
    } while (false)

namespace generic::coreModule {

    class effectSprite;
    class effect;

    static int tuple_sort(const std::tuple<ssize_t, effect*, ax::QuadCommand>& tuple1,
                          const std::tuple<ssize_t, effect*, ax::QuadCommand>& tuple2);
    static void updateUniforms(ax::backend::ProgramState* programState);


    class effect : public ax::Ref {
    public:
        ax::backend::ProgramState* getProgramState() const {
            return _programState;
        }
        virtual void setTarget(effectSprite* sprite) {}

    protected:
        bool initProgramState(const std::string& fragmentFilename);
        effect() {}
        ~effect() {
            AX_SAFE_RELEASE_NULL(_programState);
        }
        ax::backend::ProgramState* _programState = nullptr;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string _fragSource;
        ax::EventListenerCustom* _backgroundListener;
#endif
    };


    /**
     * effectSprite
     */

    class effectSprite : public ax::Sprite {
    public:
        static effectSprite* create(const std::string& filename);

        void setEffect(effect* effect);
        void addEffect(effect* effect, ssize_t order);

        void draw(ax::Renderer* renderer, const ax::Mat4& transform, uint32_t flags) override;

    protected:
        effectSprite() : _defaultEffect(nullptr) {
            _effects.reserve(2);
        }
        ~effectSprite() {
            for (auto& tuple : _effects) {
                std::get<1>(tuple)->release();
            }
            AX_SAFE_RELEASE(_defaultEffect);
        }

        std::vector<std::tuple<ssize_t, effect*, ax::QuadCommand>> _effects;
        effect* _defaultEffect;
    };

    class effectOutline : public effect {
    public:
        CREATE_FUNC(effectOutline);

        bool init() {
            initProgramState("shaders/outline.fsh");

            float radius = 0.04f;
            float threshold = 1.55f;

            SET_UNIFORM(_programState, "u_outlineColor", _outlineColor);
            SET_UNIFORM(_programState, "u_radius", radius);
            SET_UNIFORM(_programState, "u_threshold", threshold);
            SET_UNIFORM(_programState, "u_outlineAlpha", _outlineAlpha);
            _inited = true;
            return true;
        }

        void setOutlineColor(const ax::Vec3& color) {
            _outlineColor = color;
        }

        ax::Vec3 getOutlineColor() {
            return _outlineColor;
        }

        void setOutlineAlpha(float a) {
            _outlineAlpha = a;
            if (_inited) {
                SET_UNIFORM(_programState, "u_outlineAlpha", _outlineAlpha);
            }
        }

        float getOutlineAlpha() {
            return _outlineAlpha;
        }

    private:
        ax::Vec3 _outlineColor = ax::Vec3(1.0f, 0.0f, 0.0f);
        float _outlineAlpha = 1.f;
        bool _inited = false;
    };

    class spineOutline : public spine::SkeletonAnimation {
    public:
        spineOutline() : SkeletonAnimation() {
            setTwoColorTint(true);
        }

        virtual void draw(ax::Renderer* renderer, const ax::Mat4& transform, uint32_t transformFlags) override {
            if (auto outline = dynamic_cast<effectOutline*>(_effect)) {
                SkeletonAnimation::draw(renderer, transform, transformFlags);
                auto alpha = outline->getOutlineAlpha();
                outline->setOutlineAlpha(0.f);
                SkeletonAnimation::draw(renderer, transform, transformFlags);
                outline->setOutlineAlpha(alpha);
            } else {
                SkeletonAnimation::draw(renderer, transform, transformFlags);
            }
        }

        void setEffect(effect* effect) {
            _effect = effect;
        }

    private:
        effect* _effect = nullptr;
    };

}// namespace generic::coreModule


#endif// GENERIC_SHADERSNODE_H
