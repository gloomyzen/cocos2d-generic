#ifndef ANIMAL_PLANET_GRIDPROPERTY_H
#define ANIMAL_PLANET_GRIDPROPERTY_H

#include "cocos2d.h"
#include "generic/utilityModule/jsonHelper.h"
#include "propertyInterface.h"

namespace generic::coreModule {
    class gridProperty : public propertyInterface {
    public:
        gridProperty(const std::string& _propertyName) : propertyInterface(_propertyName) {}

        void parseProperty(cocos2d::Node* node, const jsonObject& object) override;
    };
}// namespace generic::coreModule


#endif// ANIMAL_PLANET_GRIDPROPERTY_H