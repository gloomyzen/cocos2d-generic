#pragma once
#ifndef GENERIC_SPRITEPROPERTY_H
#define GENERIC_SPRITEPROPERTY_H

#include "axmol.h"
#include "propertyInterface.h"

namespace generic::debugModule {
    class spriteProperty : public propertyInterface {
    public:
        spriteProperty(const std::string& value) : propertyInterface(value) {}
        ~spriteProperty();
        void viewProperty(ax::Node*) override;
        void viewPropertyFairyGUI(fairygui::GObject*) override;
    };
}// namespace generic::debugModule


#endif// GENERIC_SPRITEPROPERTY_H
