#pragma once
#ifndef GENERIC_PROPERTYINTERFACE_H
#define GENERIC_PROPERTYINTERFACE_H

#include "axmol.h"
#include "fairygui/FairyGUI.h"
#include <string>

namespace generic::debugModule {
    class propertyInterface {
    public:
        propertyInterface(const std::string& value) {
            _propertyType = value;
        }
        virtual ~propertyInterface() {}
        virtual void viewProperty(ax::Node*) {}
        virtual void viewPropertyFairyGUI(fairygui::GObject*) {}

    protected:
        std::string _propertyType;
    };
}// namespace generic::debugModule

#endif// GENERIC_PROPERTYINTERFACE_H
