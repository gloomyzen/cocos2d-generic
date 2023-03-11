#include "debugProperty.h"
#include "ImGui/imgui/imgui_internal.h"
#include "generic/coreModule/components/debugComponent.h"

using namespace generic::debugModule;
using namespace generic::coreModule;

debugProperty::~debugProperty() {}

void debugProperty::viewProperty(ax::Node* node) {
    auto dgbComponent = dynamic_cast<debugComponent*>(node->getComponent(debugComponent::DEBUG_COMPONENT_NAME));
    if (!dgbComponent) {
        dgbComponent = new debugComponent();
        node->addComponent(dgbComponent);
    }
    ImGui::SetNextItemOpen(false, ImGuiCond_Once);
    if (ImGui::CollapsingHeader(_propertyType.c_str())) {
        if (node->isRunning()) {
            auto active = dgbComponent->isDebugEnabled();
            bool tempActive = active;
            ImGui::Checkbox("Debug###enbaleDebug", &tempActive);
            if (active != tempActive) {
                dgbComponent->setDebugEnabled(tempActive);
            }
            /***
             * Debug line colors
             */
            auto dbgColor = dgbComponent->getLineColor();
            float colorRed = dbgColor.r;
            float colorGreen = dbgColor.g;
            float colorBlue = dbgColor.b;
            float colorAlpha = dbgColor.a;
            float vecColors[4] = { colorRed, colorGreen, colorBlue, colorAlpha };
            bool colorChanged = false;
            ImGui::ColorEdit4(
              "Debug color", (float*)&vecColors, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Uint8);
            if (vecColors[0] != dbgColor.r) {
                dbgColor.r = vecColors[0];
                colorChanged = true;
            }
            if (vecColors[1] != dbgColor.g) {
                dbgColor.g = vecColors[1];
                colorChanged = true;
            }
            if (vecColors[2] != dbgColor.b) {
                dbgColor.b = vecColors[2];
                colorChanged = true;
            }
            if (vecColors[3] != dbgColor.a) {
                dbgColor.a = vecColors[3];
                colorChanged = true;
            }
            if (colorChanged) {
                dgbComponent->setLineColor(dbgColor);
            }
            if (ImGui::Button("Debug node *")) {
                IM_DEBUG_BREAK();
            }
        }
    }
}

void debugProperty::viewPropertyFairyGUI(fairygui::GObject* object) {
    if (object->displayObject())
        viewProperty(object->displayObject());
}