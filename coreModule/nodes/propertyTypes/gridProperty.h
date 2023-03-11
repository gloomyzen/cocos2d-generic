#pragma once
#ifndef GENERIC_GRIDPROPERTY_H
#define GENERIC_GRIDPROPERTY_H

#include "axmol.h"
#include "generic/utilityModule/jsonHelper.h"
#include "propertyInterface.h"

namespace generic::coreModule {
    class gridProperty : public propertyInterface {
    public:
        gridProperty(const std::string& _propertyName) : propertyInterface(_propertyName) {}

        void parseProperty(ax::Node* node, const jsonObject& object) override;
    };
}// namespace generic::coreModule


#endif// GENERIC_GRIDPROPERTY_H
