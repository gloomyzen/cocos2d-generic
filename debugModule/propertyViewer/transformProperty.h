#pragma once
#ifndef GENERIC_TRANSFORMPROPERTY_H
#define GENERIC_TRANSFORMPROPERTY_H

#include "axmol.h"
#include "propertyInterface.h"

namespace generic::debugModule {
    class transformProperty : public propertyInterface {
    public:
        transformProperty(const std::string& value) : propertyInterface(value) {}
        ~transformProperty();
        void viewProperty(ax::Node*) override;
        void viewPropertyFairyGUI(fairygui::GObject*) override;
    };
}// namespace generic::debugModule


#endif// GENERIC_TRANSFORMPROPERTY_H
