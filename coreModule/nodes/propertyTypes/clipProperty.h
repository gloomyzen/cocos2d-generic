#pragma once
#ifndef GENERIC_CLIPPROPERTY_H
#define GENERIC_CLIPPROPERTY_H

#include "axmol.h"
#include "generic/utilityModule/jsonHelper.h"
#include "propertyInterface.h"

namespace generic::coreModule {
    class clipProperty : public propertyInterface {
    public:
        clipProperty(const std::string& _propertyName) : propertyInterface(_propertyName) {}

        void parseProperty(ax::Node* node, const jsonObject& object) override;
    };
}// namespace generic::coreModule


#endif// GENERIC_CLIPPROPERTY_H
