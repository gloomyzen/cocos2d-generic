#ifndef GENERIC_CLIPPROPERTY_H
#define GENERIC_CLIPPROPERTY_H

#include "cocos2d.h"
#include "generic/utilityModule/jsonHelper.h"
#include "propertyInterface.h"

namespace generic::coreModule {
    class clipProperty : public propertyInterface {
    public:
        clipProperty(const std::string& _propertyName) : propertyInterface(_propertyName) {}

        void parseProperty(cocos2d::Node* node, const jsonObject& object) override;
    };
}// namespace generic::coreModule


#endif// GENERIC_CLIPPROPERTY_H
