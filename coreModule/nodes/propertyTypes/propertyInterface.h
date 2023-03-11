#pragma once
#ifndef GENERIC_PROPERTYINTERFACE_H
#define GENERIC_PROPERTYINTERFACE_H

#include "axmol.h"
#include "generic/utilityModule/jsonHelper.h"
#include <string>

namespace generic::coreModule {
    class propertyInterface {
    public:
        propertyInterface(const std::string& _propertyName) : propertyName(_propertyName) {};
        virtual ~propertyInterface() = default;

        virtual void parseProperty(ax::Node* node, const jsonObject& object) = 0;

    protected:
        std::string propertyName;
    };
}// namespace generic::coreModule

#endif// GENERIC_PROPERTYINTERFACE_H
