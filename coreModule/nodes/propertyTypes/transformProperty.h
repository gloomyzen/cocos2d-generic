#ifndef GENERIC_TRANSFORMPROPERTY_H
#define GENERIC_TRANSFORMPROPERTY_H

#include "cocos2d.h"
#include "generic/utilityModule/jsonHelper.h"
#include "propertyInterface.h"

namespace generic::coreModule {
    class transformProperty : public propertyInterface {
    public:
        transformProperty(const std::string& _propertyName) : propertyInterface(_propertyName) {}

        void parseProperty(cocos2d::Node* node, const jsonObject& object) override;
    };
}// namespace generic::coreModule


#endif// GENERIC_TRANSFORMPROPERTY_H
