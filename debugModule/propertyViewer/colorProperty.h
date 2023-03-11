#pragma once
#ifndef GENERIC_COLORPROPERTY_H
#define GENERIC_COLORPROPERTY_H

#include "axmol.h"
#include "propertyInterface.h"

namespace generic::debugModule {
    class colorProperty : public propertyInterface {
    public:
        colorProperty(const std::string& value) : propertyInterface(value) {}
        ~colorProperty();
        void viewProperty(ax::Node*) override;
        void viewPropertyFairyGUI(fairygui::GObject*) override;
    };
}// namespace generic::debugModule


#endif// GENERIC_COLORPROPERTY_H
