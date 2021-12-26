#ifndef GENERIC_ASEPRITEPROPERTY_H
#define GENERIC_ASEPRITEPROPERTY_H

#include "cocos2d.h"
#include "generic/utilityModule/jsonHelper.h"
#include "propertyInterface.h"

namespace generic::coreModule {
    class asepriteProperty : public propertyInterface {
    public:
        asepriteProperty(const std::string& _propertyName) : propertyInterface(_propertyName) {}

        void parseProperty(cocos2d::Node* node, const jsonObject& object) override;
    };
}// namespace generic::coreModule


#endif// GENERIC_ASEPRITEPROPERTY_H
