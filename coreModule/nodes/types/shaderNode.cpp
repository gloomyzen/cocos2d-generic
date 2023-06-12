#include "shaderNode.h"
#include <renderer/backend/Device.h>

using namespace generic::coreModule;
using namespace ax;

int generic::coreModule::tuple_sort(const std::tuple<ssize_t, effect*, QuadCommand>& tuple1,
                                    const std::tuple<ssize_t, effect*, QuadCommand>& tuple2) {
    return std::get<0>(tuple1) < std::get<0>(tuple2);
}

void generic::coreModule::updateUniforms(backend::ProgramState* programState) {
    float time = Director::getInstance()->getTotalFrames() * Director::getInstance()->getAnimationInterval();
    Vec4 uTime(time / 10.0f, time, time * 2.0f, time * 4.0f);
    Vec4 sinTime(time / 8.0f, time / 4.0f, time / 2.0f, sinf(time));
    Vec4 cosTime(time / 8.0f, time / 4.0f, time / 2.0f, cosf(time));

    SET_UNIFORM(programState, "u_Time", uTime);
    SET_UNIFORM(programState, "u_SinTime", sinTime);
    SET_UNIFORM(programState, "u_CosTime", cosTime);
}


bool effect::initProgramState(const std::string& fragmentFilename) {
    auto fileUtils = FileUtils::getInstance();
    auto fragmentFullPath = fileUtils->fullPathForFilename(fragmentFilename);
    auto fragSource = fileUtils->getStringFromFile(fragmentFullPath);

#if (AX_TARGET_PLATFORM == AX_PLATFORM_ANDROID)
    _fragSource = fragSource;
#endif
    auto program = backend::Device::getInstance()->newProgram(positionTextureColor_vert, fragSource);
    auto programState = new backend::ProgramState(program);
    CC_SAFE_RELEASE(_programState);
    CC_SAFE_RELEASE(program);
    _programState = programState;

    return _programState != nullptr;
}

effectSprite* effectSprite::create(const std::string& filename) {
    auto ret = new (std::nothrow) effectSprite;
    if (ret && ret->initWithFile(filename)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_RELEASE(ret);
    return nullptr;
}

void effectSprite::setEffect(effect* effect) {
    if (_defaultEffect != effect) {
        effect->setTarget(this);

        CC_SAFE_RELEASE(_defaultEffect);
        _defaultEffect = effect;
        AX_SAFE_RETAIN(_defaultEffect);

        setProgramState(_defaultEffect->getProgramState());
    }
}

void effectSprite::addEffect(effect* effect, ssize_t order) {
    effect->retain();
    effect->setTarget(this);

    _effects.emplace_back(order, effect, ax::QuadCommand());

    std::sort(std::begin(_effects), std::end(_effects), tuple_sort);
}

void effectSprite::draw(ax::Renderer* renderer, const Mat4& transform, uint32_t flags) {
#if CC_USE_CULLING
    // Don't do calculate the culling if the transform was not updated
    _insideBounds = (flags & FLAGS_TRANSFORM_DIRTY) ? renderer->checkVisibility(transform, _contentSize) : _insideBounds;

    if (_insideBounds)
#endif
    {
        // negative effects: order < 0
        int idx = 0;
        for (auto& effect : _effects) {

            if (std::get<0>(effect) >= 0)
                break;
            auto* programState = std::get<1>(effect)->getProgramState();
            if (programState) {
                ax::QuadCommand& q = std::get<2>(effect);
                q.init(_globalZOrder, _texture, _blendFunc, &_quad, 1, transform, flags);
                updateUniforms(programState);
                renderer->addCommand(&q);
            }
            idx++;
        }

        // normal effect: order == 0
        _trianglesCommand.init(_globalZOrder, _texture, _blendFunc, _polyInfo.triangles, transform, flags);

        updateUniforms(_trianglesCommand.getPipelineDescriptor().programState);
        renderer->addCommand(&_trianglesCommand);

        // positive effects: order >= 0
        for (auto it = std::begin(_effects) + idx; it != std::end(_effects); ++it) {
            ax::QuadCommand& q = std::get<2>(*it);
            auto* programState = std::get<1>(*it)->getProgramState();
            updateUniforms(programState);
            q.init(_globalZOrder, _texture, _blendFunc, &_quad, 1, transform, flags);
            q.getPipelineDescriptor().programState = programState;
            renderer->addCommand(&q);
            idx++;
        }
    }
}
