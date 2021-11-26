#ifndef ANIMAL_PLANET_SPRITEPROPERTY_H
#define ANIMAL_PLANET_SPRITEPROPERTY_H

#include "cocos2d.h"
#include "generic/utilityModule/jsonHelper.h"
#include "propertyInterface.h"

namespace generic::coreModule {
    class spriteProperty : public propertyInterface {
    public:
        spriteProperty(const std::string& _propertyName) : propertyInterface(_propertyName) {}

        void parseProperty(cocos2d::Node* node, const jsonObject& object) override;
    };
}// namespace generic::coreModule


#endif// ANIMAL_PLANET_SPRITEPROPERTY_H
