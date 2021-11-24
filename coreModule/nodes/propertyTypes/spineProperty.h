#ifndef ANIMAL_PLANET_SPINEPROPERTY_H
#define ANIMAL_PLANET_SPINEPROPERTY_H

#include "cocos2d.h"
#include "generic/utilityModule/jsonHelper.h"
#include "propertyInterface.h"

namespace generic::coreModule {
    class spineProperty : public propertyInterface {
    public:
        ~spineProperty() override = 0;

        void parseProperty(cocos2d::Node* node, const jsonObject& object) override;
    };
}// namespace generic::coreModule


#endif// ANIMAL_PLANET_SPINEPROPERTY_H
