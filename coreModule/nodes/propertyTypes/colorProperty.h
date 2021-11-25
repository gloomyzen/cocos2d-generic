#ifndef ANIMAL_PLANET_COLORPROPERTY_H
#define ANIMAL_PLANET_COLORPROPERTY_H

#include "cocos2d.h"
#include "generic/utilityModule/jsonHelper.h"
#include "propertyInterface.h"

namespace generic::coreModule {
    class colorProperty : public propertyInterface {
    public:
        colorProperty(const std::string& _propertyName) : propertyInterface(_propertyName) {}
        ~colorProperty() override {}

        void parseProperty(cocos2d::Node* node, const jsonObject& object) override;
    };
}// namespace generic::coreModule


#endif// ANIMAL_PLANET_COLORPROPERTY_H
