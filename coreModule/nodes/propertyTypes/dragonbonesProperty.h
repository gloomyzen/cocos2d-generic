#ifndef ANIMAL_PLANET_DRAGONBONESPROPERTY_H
#define ANIMAL_PLANET_DRAGONBONESPROPERTY_H


#include "cocos2d.h"
#include "generic/utilityModule/jsonHelper.h"
#include "propertyInterface.h"
#include <map>
#include <string>

namespace generic::coreModule {
    class dragonbonesProperty : public propertyInterface {
    public:
        dragonbonesProperty(const std::string& _propertyName) : propertyInterface(_propertyName) {}
        ~dragonbonesProperty() override {}

        void parseProperty(cocos2d::Node* node, const jsonObject& object) override;
    private:
        std::map<std::string, bool> cachedArmature;
    };
}// namespace generic::coreModule


#endif// ANIMAL_PLANET_DRAGONBONESPROPERTY_H
