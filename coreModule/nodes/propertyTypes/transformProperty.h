#pragma once
#ifndef GENERIC_TRANSFORMPROPERTY_H
#define GENERIC_TRANSFORMPROPERTY_H

#include "axmol.h"
#include "generic/utilityModule/jsonHelper.h"
#include "propertyInterface.h"

namespace generic::coreModule {
    class baseProperty : public propertyInterface {
    public:
        baseProperty(const std::string& _propertyName) : propertyInterface(_propertyName) {}

        void parseProperty(ax::Node* node, const jsonObject& object) override;
    };
}// namespace generic::coreModule


#endif// GENERIC_TRANSFORMPROPERTY_H
