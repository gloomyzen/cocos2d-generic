#ifndef GENERIC_SPINEPROPERTY_H
#define GENERIC_SPINEPROPERTY_H

#include "cocos2d.h"
#include "generic/utilityModule/jsonHelper.h"
#include "propertyInterface.h"

namespace generic::coreModule {
    class spineProperty : public propertyInterface {
    public:
        spineProperty(const std::string& _propertyName) : propertyInterface(_propertyName) {}

        void parseProperty(cocos2d::Node* node, const jsonObject& object) override;
    };
}// namespace generic::coreModule


#endif// GENERIC_SPINEPROPERTY_H
