#include "propertyViewer.h"
#include "propertyInterface.h"
#include "baseProperty.h"
#include "transformProperty.h"
#include "colorProperty.h"
#include "spriteProperty.h"
#include "spineProperty.h"
#include "labelProperty.h"
#include "debugProperty.h"
#include "fairyProperty.h"
#include "axmol.h"
#include "FairyGUI.h"

using namespace generic::debugModule;

propertyViewer::propertyViewer() {
    _propertiesList.emplace_back(new baseProperty("Base"));
    _propertiesList.emplace_back(new transformProperty("Transform"));
    _propertiesList.emplace_back(new debugProperty("Debug"));
    _propertiesList.emplace_back(new colorProperty("Color"));
    _propertiesList.emplace_back(new spriteProperty("Sprite"));
    _propertiesList.emplace_back(new spineProperty("Spine"));
    _propertiesList.emplace_back(new labelProperty("Label"));
    _propertiesList.emplace_back(new fairyProperty("FairyGUI"));
}

propertyViewer::~propertyViewer() {
    for (auto& item : _propertiesList) {
        delete item;
    }
    _propertiesList.clear();
}

void propertyViewer::update(ax::Node* node) {
    if (!node)
        return;
    for (const auto& property : _propertiesList) {
        property->viewProperty(node);
    }
}

void propertyViewer::updateFairy(fairygui::GObject* node) {
    if (!node)
        return;
    for (const auto& property : _propertiesList) {
        property->viewPropertyFairyGUI(node);
    }
}
