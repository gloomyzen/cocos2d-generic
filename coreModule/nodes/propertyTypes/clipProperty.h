#ifndef ANIMAL_PLANET_CLIPPROPERTY_H
#define ANIMAL_PLANET_CLIPPROPERTY_H

#include "cocos2d.h"
#include "generic/utilityModule/jsonHelper.h"
#include "propertyInterface.h"

namespace generic::coreModule {
    class clipProperty : public propertyInterface {
    public:
        clipProperty(const std::string& _propertyName) : propertyInterface(_propertyName) {}
        ~clipProperty() override {}

        void parseProperty(cocos2d::Node* node, const jsonObject& object) override;
    };
}// namespace generic::coreModule


#endif// ANIMAL_PLANET_CLIPPROPERTY_H
