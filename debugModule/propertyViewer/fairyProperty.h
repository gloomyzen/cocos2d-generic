#pragma once
#ifndef GENERIC_FAIRYPROPERTY_H
#define GENERIC_FAIRYPROPERTY_H

#include "axmol.h"
#include "propertyInterface.h"
#include <vector>

namespace generic::debugModule {
    class fairyProperty : public propertyInterface {
    public:
        fairyProperty(const std::string& value) : propertyInterface(value) {}
        ~fairyProperty();
        void viewProperty(ax::Node*) override;
        void viewPropertyFairyGUI(fairygui::GObject*) override;
    private:
        void proceedObject(fairygui::GObject*);
        static std::vector<fairygui::RelationType> _relationTypesList;
    };
}// namespace generic::debugModule


#endif// GENERIC_FAIRYPROPERTY_H
