#ifndef ANIMAL_PLANET_SCALE9SPRITEPROPERTY_H
#define ANIMAL_PLANET_SCALE9SPRITEPROPERTY_H

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


#endif// ANIMAL_PLANET_SCALE9SPRITEPROPERTY_H
