#ifndef GENERIC_SCALE9SPRITEPROPERTY_H
#define GENERIC_SCALE9SPRITEPROPERTY_H

#include "cocos2d.h"
#include "generic/utilityModule/jsonHelper.h"
#include "propertyInterface.h"

namespace generic::coreModule {
    class scale9SpriteProperty : public propertyInterface {
    public:
        scale9SpriteProperty(const std::string& _propertyName) : propertyInterface(_propertyName) {}

        void parseProperty(cocos2d::Node* node, const jsonObject& object) override;
    };
}// namespace generic::coreModule


#endif// GENERIC_SCALE9SPRITEPROPERTY_H
