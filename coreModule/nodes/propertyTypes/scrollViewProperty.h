#pragma once
#ifndef GENERIC_SCROLLVIEWPROPERTY_H
#define GENERIC_SCROLLVIEWPROPERTY_H

#include "axmol.h"
#include "generic/utilityModule/jsonHelper.h"
#include "propertyInterface.h"
#include "ui/CocosGUI.h"
#include <string>
#include <map>

namespace generic::coreModule {
    class scrollViewProperty : public propertyInterface {
    public:
        scrollViewProperty(const std::string& _propertyName) : propertyInterface(_propertyName) {}

        void parseProperty(ax::Node* node, const jsonObject& object) override;

    private:
        static std::map<std::string, ax::ui::ScrollView::Direction> scrollDirectionsMap;
    };
}// namespace generic::coreModule


#endif// GENERIC_SCROLLVIEWPROPERTY_H
