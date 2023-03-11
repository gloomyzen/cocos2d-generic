#include "spriteProperty.h"
#include "ImGui/imgui/imgui_internal.h"
#include "axmol.h"

using namespace generic::debugModule;

spriteProperty::~spriteProperty() {}

void spriteProperty::viewProperty(ax::Node* node) {
    if (auto spriteNode = dynamic_cast<ax::Sprite*>(node)) {
        ImGui::SetNextItemOpen(false, ImGuiCond_Once);
        if (ImGui::CollapsingHeader(_propertyType.c_str())) {
            ImGui::Text("Image path \"%s\"", spriteNode->getResourceName().data());
        }
    }
}

void spriteProperty::viewPropertyFairyGUI(fairygui::GObject*) {
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader(_propertyType.c_str())) {
        //
    }
}



