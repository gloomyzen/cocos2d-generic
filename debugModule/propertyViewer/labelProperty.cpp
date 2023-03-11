#include "labelProperty.h"
#include "ImGui/imgui/imgui_internal.h"
#include "axmol.h"

using namespace generic::debugModule;

labelProperty::~labelProperty() {}

void labelProperty::viewProperty(ax::Node* node) {
    if (auto labelNode = dynamic_cast<ax::Label*>(node)) {
        ImGui::SetNextItemOpen(false, ImGuiCond_Once);
        if (ImGui::CollapsingHeader(_propertyType.c_str())) {
            // todo lh ls color text font shadow
            /*** Text */
            auto labelText = labelNode->getString();
            char* newLabelText = const_cast<char*>(labelText.data());
            ImGui::InputText("Text", newLabelText, IM_ARRAYSIZE(newLabelText) * 16);
            if (labelText != newLabelText) {
                labelNode->setString(newLabelText);
            }
            auto labelWidth = labelNode->getMaxLineWidth();
            auto tempWidth = labelWidth;
            float changedWidth = labelWidth;
            ImGui::DragFloat("Max Line Width", &changedWidth, 1.f, 0.f, 9999.f);
            if (changedWidth != tempWidth) {
                labelNode->setMaxLineWidth(changedWidth);
            }
            /*** Label Effects todo not working! */
            /*int labelEffectsIdx = 0;
            auto labelEffect = labelNode->getLabelEffectType();
            for (int i = 0; i < labelEffectList.size(); ++i) {
                if (labelEffectList[i].second == labelEffect) {
                    labelEffectsIdx = i;
                }
            }
            int currentEffectsIdx = labelEffectsIdx;
            std::string elementsList;
            for (const auto &item : labelEffectList) {
                elementsList += item.first + '\0';
            }
            if (ImGui::Combo("Style", &labelEffectsIdx, elementsList.c_str())) {
                if (labelEffectsIdx != currentEffectsIdx) {
                    //
                }
            }*/
        }
    }
}

void labelProperty::viewPropertyFairyGUI(fairygui::GObject*) {
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader(_propertyType.c_str())) {
        //
    }
}