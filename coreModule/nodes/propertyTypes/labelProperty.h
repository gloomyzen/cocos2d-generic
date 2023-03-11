#pragma once
#ifndef GENERIC_LABELPROPERTY_H
#define GENERIC_LABELPROPERTY_H

#include "axmol.h"
#include "generic/utilityModule/jsonHelper.h"
#include "propertyInterface.h"
#include <string>
#include <map>

namespace generic::coreModule {
    class labelProperty : public propertyInterface {
    public:
        labelProperty(const std::string& _propertyName) : propertyInterface(_propertyName) {}

        void parseProperty(ax::Node* node, const jsonObject& object) override;

    private:
        static std::string defaultFont;
        static std::map<std::string, ax::TextHAlignment> textHAlignmentTypes;
    };
}// namespace generic::coreModule


#endif// GENERIC_LABELPROPERTY_H
