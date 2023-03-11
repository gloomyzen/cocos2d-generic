#pragma once
#ifndef GENERIC_DRAGONBONESPROPERTY_H
#define GENERIC_DRAGONBONESPROPERTY_H


#include "axmol.h"
#include "generic/utilityModule/jsonHelper.h"
#include "propertyInterface.h"
#include <map>
#include <string>

namespace generic::coreModule {
    class dragonbonesProperty : public propertyInterface {
    public:
        dragonbonesProperty(const std::string& _propertyName) : propertyInterface(_propertyName) {}

        void parseProperty(ax::Node* node, const jsonObject& object) override;
    private:
        std::map<std::string, bool> cachedArmature;
    };
}// namespace generic::coreModule


#endif// GENERIC_DRAGONBONESPROPERTY_H
