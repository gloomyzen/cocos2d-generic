#pragma once
#ifndef GENERIC_LABELPROPERTY_H
#define GENERIC_LABELPROPERTY_H

#include "axmol.h"
#include "propertyInterface.h"

namespace generic::debugModule {
    class labelProperty : public propertyInterface {
    public:
        labelProperty(const std::string& value) : propertyInterface(value) {}
        ~labelProperty();
        void viewProperty(ax::Node*) override;
        void viewPropertyFairyGUI(fairygui::GObject*) override;
    };
}// namespace generic::debugModule


#endif// GENERIC_LABELPROPERTY_H
