#include "transformProperty.h"
#include "ImGui/imgui/imgui_internal.h"
#include "generic/debugModule/imguiController.h"
#include "generic/coreModule/components/transformComponent.h"
#include <functional>

using namespace generic::debugModule;
using namespace generic::coreModule;

transformProperty::~transformProperty() {}

void transformProperty::viewProperty(ax::Node* node) {
    auto transform = dynamic_cast<transformComponent*>(node->getComponent(transformComponent::TRANSFORM_COMPONENT_NAME));
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader(_propertyType.c_str())) {
        /**
         * Height and Width
         */
        auto width = node->getContentSize().width;
        auto height = node->getContentSize().height;
        float vecWH[2] = { width, height };
        ImGui::DragFloat2("Width/Height", vecWH, 1);
        if (vecWH[0] != width || vecWH[1] != height) {
            auto _size = ax::Size();
            _size.width = vecWH[0];
            _size.height = vecWH[1];
            node->setContentSize(_size);
        }
        /**
         * Position by X Y Z
         */
        auto nodePos = node->getPosition3D();
        float vecPos[3] = { nodePos.x, nodePos.y, nodePos.z };
        ImGui::DragFloat3("Position X/Y", vecPos, 1.f);
        if (vecPos[0] != nodePos.x || vecPos[1] != nodePos.y || vecPos[2] != nodePos.z) {
            ax::Vec3 vec3 = { vecPos[0], vecPos[1], vecPos[2] };
            node->setPosition3D(vec3);
        }
        if (transform) {
            /**
             * Pivot
             */
            auto nodePivot = transform->getPivotPoint();
            float vecPivot[2] = { nodePivot.x, nodePivot.y };
            ImGui::DragFloat2("Pivot X/Y", vecPivot, 0.01f, -10.f, 10.f);
            if (vecPivot[0] != nodePivot.x || vecPivot[1] != nodePivot.y) {
                transform->setPivotPoint({ vecPivot[0], vecPivot[1] });
            }
            /**
             * Stretch
             */
            auto nodeStretch = transform->getStretch();
            float vecStretch[2] = { nodeStretch.x, nodeStretch.y };
            ImGui::DragFloat2("Stretch W/H", vecStretch, 0.01f, 0.f, 10.f);
            if (vecStretch[0] != nodeStretch.x || vecStretch[1] != nodeStretch.y) {
                transform->setStretch(vecStretch[0], vecStretch[1]);
            }
        }
        /**
         * Anchor
         */
        auto nodeAnchor = node->getAnchorPoint();
        float vecAnchor[2] = { nodeAnchor.x, nodeAnchor.y };
        ImGui::DragFloat2("Anchor X/Y", vecAnchor, 0.01f, -100.f, 100.f);
        if (vecAnchor[0] != nodeAnchor.x || vecAnchor[1] != nodeAnchor.y) {
            node->setAnchorPoint({ vecAnchor[0], vecAnchor[1] });
        }
        /**
         * Scale
         */
        auto nodeScaleX = node->getScaleX();
        auto nodeScaleY = node->getScaleY();
        auto tempScaleX = nodeScaleX;
        auto tempScaleY = nodeScaleY;
        float vecScale[2] = { nodeScaleX, nodeScaleY };
        ImGui::DragFloat2("Scale X/Y", vecScale, 0.01f);
        if (tempScaleX != vecScale[0] || tempScaleY != vecScale[1]) {
            node->setScaleX(vecScale[0]);
            node->setScaleY(vecScale[1]);
        }
        /**
         * Skew X and Y
         */
        auto nodeSkewX = node->getSkewX();
        auto nodeSkewY = node->getSkewY();
        auto tempSkewX = nodeSkewX;
        auto tempSkewY = nodeSkewY;
        float vecSkew[2] = { nodeSkewX, nodeSkewY };
        ImGui::DragFloat2("Skew X/Y", vecSkew, 0.1f);
        if (tempSkewX != vecSkew[0] || tempSkewY != vecSkew[1]) {
            node->setSkewX(vecSkew[0]);
            node->setSkewY(vecSkew[1]);
        }
        /**
         * Rotation
         */
        auto nodeRotation = node->getRotation();
        auto tempRotation = nodeRotation;
        float floatRotation[1] = { nodeRotation };
        ImGui::DragFloat("Rotation", floatRotation, 0.1f);
        if (floatRotation[0] != tempRotation) {
            node->setRotation(floatRotation[0]);
        }
        auto local = node->getLocalZOrder();
        int changedLocal = local;
        ImGui::DragInt("Local z-order", &changedLocal, 1, -999, 999);
        if (changedLocal != local) {
            node->setLocalZOrder(static_cast<std::int32_t>(changedLocal));
        }
    }
}

void transformProperty::viewPropertyFairyGUI(fairygui::GObject* node) {
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader(_propertyType.c_str())) {
        /**
         * Height and Width
         */
        GET_IMGUI_CONTROLLER().dragFloat("Size", node->getSize(), 1, [&node](const ax::Size& val){
            node->setSize(val.x, val.y);
        });
        GET_IMGUI_CONTROLLER().dragFloat("Position", node->getPosition(), 1, [&node](const ax::Size& val){
            node->setPosition(val.x, val.y);
        });
        GET_IMGUI_CONTROLLER().dragFloat("Pivot", node->getPivot(), 1, [&node](const ax::Size& val){
            node->setPivot(val.x, val.y);
        });
        GET_IMGUI_CONTROLLER().dragFloat("Scale", node->getScale(), 1, [&node](const ax::Size& val){
            node->setScale(val.x, val.y);
        });
        std::vector<float> skew = {node->getSkewX(), node->getSkewY()};
        GET_IMGUI_CONTROLLER().dragFloat("Skew", skew, 1, [&node](const std::vector<float>& val){
            node->setSkewX(val[0]);
            node->setSkewY(val[1]);
        });
//        /**
//         * Rotation
//         */
//        auto nodeRotation = node->getRotation();
//        auto tempRotation = nodeRotation;
//        float floatRotation[1] = { nodeRotation };
//        ImGui::DragFloat("Rotation", floatRotation, 0.1f);
//        if (floatRotation[0] != tempRotation) {
//            node->setRotation(floatRotation[0]);
//        }
//        auto local = node->getLocalZOrder();
//        int changedLocal = local;
//        ImGui::DragInt("Local z-order", &changedLocal, 1, -999, 999);
//        if (changedLocal != local) {
//            node->setLocalZOrder(static_cast<std::int32_t>(changedLocal));
//        }
    }
}

