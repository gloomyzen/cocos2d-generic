#pragma once
#ifndef GENERIC_SPINEPROPERTY_H
#define GENERIC_SPINEPROPERTY_H

#include "axmol.h"
#include "propertyInterface.h"

namespace generic::debugModule {
    class spineProperty : public propertyInterface {
    public:
        spineProperty(const std::string& value) : propertyInterface(value) {}
        ~spineProperty();
        void viewProperty(ax::Node*) override;
        void viewPropertyFairyGUI(fairygui::GObject*) override;
    };
}// namespace generic::debugModule


#endif// GENERIC_SPINEPROPERTY_H
