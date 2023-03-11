#pragma once
#ifndef GENERIC_BASEPROPERTY_H
#define GENERIC_BASEPROPERTY_H

#include "axmol.h"
#include "propertyInterface.h"

namespace generic::debugModule {
    class baseProperty : public propertyInterface {
    public:
        baseProperty(const std::string& value) : propertyInterface(value) {}
        ~baseProperty();
        void viewProperty(ax::Node*) override;
        void viewPropertyFairyGUI(fairygui::GObject*) override;
    };
}// namespace generic::debugModule


#endif// GENERIC_BASEPROPERTY_H
