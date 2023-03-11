#include "baseProperty.h"
#include "ImGui/imgui/imgui_internal.h"

using namespace generic::debugModule;

baseProperty::~baseProperty() {}

void baseProperty::viewProperty(ax::Node* node) {
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader(_propertyType.c_str())) {
        ImGui::Text("Node Name(ID) %s", node->getName().data());
        ImGui::Text("Node GUI %d", node->_ID);
        if (node->isRunning()) {
            auto active = node->isVisible();
            auto tempActive = active;
            ImGui::Checkbox(" Is active", &active);
            if (active != tempActive) {
                node->setVisible(active);
            }
        }
    }
}

void baseProperty::viewPropertyFairyGUI(fairygui::GObject* node) {
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader(_propertyType.c_str())) {
        ImGui::Text("Node Name(ID) %s", node->name.c_str());
        ImGui::Text("Node GUI %s", node->id.c_str());
        auto active = node->isVisible();
        auto tempActive = active;
        ImGui::Checkbox(" Is active", &active);
        if (active != tempActive) {
            node->setVisible(active);
        }
    }
}
