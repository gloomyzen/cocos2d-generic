#pragma once
#ifndef GENERIC_SPINEPROPERTY_H
#define GENERIC_SPINEPROPERTY_H

#include "axmol.h"
#include "generic/utilityModule/jsonHelper.h"
#include "propertyInterface.h"

namespace generic::coreModule {
    class spineProperty : public propertyInterface {
    public:
        spineProperty(const std::string& _propertyName) : propertyInterface(_propertyName) {}

        void parseProperty(ax::Node* node, const jsonObject& object) override;
    };
}// namespace generic::coreModule


#endif// GENERIC_SPINEPROPERTY_H
