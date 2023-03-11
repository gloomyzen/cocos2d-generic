#include "colorProperty.h"
#include "ImGui/imgui/imgui_internal.h"

using namespace generic::debugModule;

colorProperty::~colorProperty() {}

void colorProperty::viewProperty(ax::Node* node) {
    ImGui::SetNextItemOpen(false, ImGuiCond_Once);
    if (ImGui::CollapsingHeader(_propertyType.c_str())) {
        /**
         * Set cascade opacity
         */
        auto isCascadeOpacity = node->isCascadeOpacityEnabled();
        auto tempCascadeOpacity = isCascadeOpacity;
        ImGui::Checkbox("Cascade opacity", &tempCascadeOpacity);
        if (tempCascadeOpacity != isCascadeOpacity) {
            node->setCascadeOpacityEnabled(tempCascadeOpacity);
        }
        /**
         * Set opacity
         */
        auto nodeOpacity = node->getOpacity();
        auto tempOpacity = nodeOpacity;
        int changedOpacity = static_cast<int>(nodeOpacity);
        ImGui::DragInt("Opacity", &changedOpacity, 1, 0, 255);
        if (changedOpacity != tempOpacity) {
            node->setOpacity(static_cast<uint8_t>(changedOpacity));
        }
        /**
         * Set cascade color
         */
        auto isCascadeColor = node->isCascadeColorEnabled();
        auto tempCascadeColor = isCascadeColor;
        ImGui::Checkbox("Cascade color", &tempCascadeColor);
        if (tempCascadeColor != isCascadeColor) {
            node->setCascadeColorEnabled(tempCascadeColor);
        }
        /***
         * Sprite Color
         */
        auto nodeColor = node->getColor();
        float nodeRed = nodeColor.r / 255.0f;
        float nodeGreen = nodeColor.g / 255.0f;
        float nodeBlue = nodeColor.b / 255.0f;
        float nodeAlpha = node->getOpacity() / 255.0f;
        float vecColors[4] = { nodeRed, nodeGreen, nodeBlue, nodeAlpha };
        ImGui::ColorEdit4(
          "Color", (float*)&vecColors, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Uint8);
        if (vecColors[0] != static_cast<float>(nodeColor.r) / 255.0f) {
            nodeColor.r = static_cast<uint8_t>(vecColors[0] * 255.0f);
            if (nodeColor.r < 0)
                nodeColor.r = 0;
            else if (nodeColor.r >= 255)
                nodeColor.r = 255;
            node->setColor(nodeColor);
        }
        if (vecColors[1] != static_cast<float>(nodeColor.g) / 255.0f) {
            nodeColor.g = static_cast<uint8_t>(vecColors[1] * 255.0f);
            if (nodeColor.g < 0)
                nodeColor.g = 0;
            else if (nodeColor.g >= 255)
                nodeColor.g = 255;
            node->setColor(nodeColor);
        }
        if (vecColors[2] != static_cast<float>(nodeColor.b) / 255.0f) {
            nodeColor.b = static_cast<uint8_t>(vecColors[2] * 255.0f);
            if (nodeColor.b < 0)
                nodeColor.b = 0;
            else if (nodeColor.b >= 255)
                nodeColor.b = 255;
            node->setColor(nodeColor);
        }
        if (vecColors[3] != static_cast<float>(nodeAlpha) / 255.0f) {
            nodeAlpha = static_cast<uint8_t>(vecColors[3] * 255.0f);
            if (nodeAlpha < 0)
                nodeAlpha = 0;
            else if (nodeAlpha >= 255)
                nodeAlpha = 255;
            node->setOpacity(static_cast<uint8_t>(nodeAlpha));
        }
    }
}

void colorProperty::viewPropertyFairyGUI(fairygui::GObject*) {
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader(_propertyType.c_str())) {
        //
    }
}