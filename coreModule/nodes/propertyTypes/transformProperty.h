#ifndef ANIMAL_PLANET_TRANSFORMPROPERTY_H
#define ANIMAL_PLANET_TRANSFORMPROPERTY_H

#include "cocos2d.h"
#include "generic/utilityModule/jsonHelper.h"
#include "propertyInterface.h"

namespace generic::coreModule {
    class transformProperty : public propertyInterface {
    public:
        transformProperty(const std::string& _propertyName) : propertyInterface(_propertyName) {}
        ~transformProperty() override {}

        void parseProperty(cocos2d::Node* node, const jsonObject& object) override;
    };
}// namespace generic::coreModule


#endif// ANIMAL_PLANET_TRANSFORMPROPERTY_H
