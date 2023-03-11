#pragma once
#ifndef GENERIC_DEBUGPROPERTY_H
#define GENERIC_DEBUGPROPERTY_H

#include "axmol.h"
#include "propertyInterface.h"

namespace generic::debugModule {
    class debugProperty : public propertyInterface {
    public:
        debugProperty(const std::string& value) : propertyInterface(value) {}
        ~debugProperty();
        void viewProperty(ax::Node*) override;
        void viewPropertyFairyGUI(fairygui::GObject*) override;
    };
}// namespace generic::debugModule


#endif// GENERIC_DEBUGPROPERTY_H
