#pragma once
#ifndef GENERIC_COLORPROPERTY_H
#define GENERIC_COLORPROPERTY_H

#include "axmol.h"
#include "generic/utilityModule/jsonHelper.h"
#include "propertyInterface.h"

namespace generic::coreModule {
    class colorProperty : public propertyInterface {
    public:
        colorProperty(const std::string& _propertyName) : propertyInterface(_propertyName) {}

        void parseProperty(ax::Node* node, const jsonObject& object) override;
    };
}// namespace generic::coreModule


#endif// GENERIC_COLORPROPERTY_H
