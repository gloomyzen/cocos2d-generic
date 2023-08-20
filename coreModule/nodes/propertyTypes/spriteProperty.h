#pragma once
#ifndef GENERIC_SPRITEPROPERTY_H
#define GENERIC_SPRITEPROPERTY_H

#include "axmol.h"
#include "generic/utilityModule/jsonHelper.h"
#include "propertyInterface.h"
#include <map>
#include <string>

namespace generic::coreModule {
    class spriteProperty : public propertyInterface {
    public:
        spriteProperty(const std::string& _propertyName) : propertyInterface(_propertyName) {}

        void parseProperty(ax::Node* node, const jsonObject& object) override;


    private:
#ifdef CC_SPRITE_SET_RENDERMODE
        static std::map<std::string, cocos2d::Sprite::RenderMode> renderModeMap;
#endif
    };
}// namespace generic::coreModule


#endif// GENERIC_SPRITEPROPERTY_H
