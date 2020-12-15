#include "spriteParameters.h"

using namespace common::coreModule;

static cocos2d::Texture2D::TexParams pixelartTexParams = {
		cocos2d::backend::SamplerFilter::NEAREST,       // TextureMinFilter
		cocos2d::backend::SamplerFilter::NEAREST,       // TextureMagFilter
		cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE, // TextureWrapMode Horizontal
		cocos2d::backend::SamplerAddressMode::CLAMP_TO_EDGE  // TextureWrapMode Vertical
};

void spriteParameters::setCorrectPixelartTexture(cocos2d::Sprite *sprite) {
	if (sprite->getTexture() != nullptr)
		sprite->getTexture()->setTexParameters(pixelartTexParams);
}
