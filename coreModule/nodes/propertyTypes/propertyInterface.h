#ifndef ANIMAL_PLANET_PROPERTYINTERFACE_H
#define ANIMAL_PLANET_PROPERTYINTERFACE_H

#include "cocos2d.h"
#include "generic/utilityModule/jsonHelper.h"
#include <string>

namespace generic::coreModule {
    class propertyInterface {
    public:
        propertyInterface(const std::string& _propertyName) : propertyName(_propertyName) {};
        virtual ~propertyInterface() = 0;

        virtual void parseProperty(cocos2d::Node* node, const jsonObject& object) = 0;

    protected:
        std::string propertyName;
    };
}// namespace generic::coreModule

#endif// ANIMAL_PLANET_PROPERTYINTERFACE_H
