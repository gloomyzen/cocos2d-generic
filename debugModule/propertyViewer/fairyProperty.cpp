#include "fairyProperty.h"
#include "ImGui/imgui/imgui_internal.h"
#include "axmol.h"
#include "FairyGUI.h"

using namespace generic::debugModule;
using namespace fairygui;

std::vector<fairygui::RelationType> fairyProperty::_relationTypesList = { fairygui::RelationType::Left_Left, fairygui::RelationType::Left_Center,
    fairygui::RelationType::Left_Right, fairygui::RelationType::Center_Center, fairygui::RelationType::Right_Left, fairygui::RelationType::Right_Center,
    fairygui::RelationType::Right_Right, fairygui::RelationType::Top_Top, fairygui::RelationType::Top_Middle, fairygui::RelationType::Top_Bottom,
    fairygui::RelationType::Middle_Middle, fairygui::RelationType::Bottom_Top, fairygui::RelationType::Bottom_Middle, fairygui::RelationType::Bottom_Bottom,
    fairygui::RelationType::Width, fairygui::RelationType::Height, fairygui::RelationType::LeftExt_Left, fairygui::RelationType::LeftExt_Right,
    fairygui::RelationType::RightExt_Left, fairygui::RelationType::RightExt_Right, fairygui::RelationType::TopExt_Top, fairygui::RelationType::TopExt_Bottom,
    fairygui::RelationType::BottomExt_Top, fairygui::RelationType::BottomExt_Bottom, fairygui::RelationType::Size
};

fairyProperty::~fairyProperty() {}

void fairyProperty::viewProperty(ax::Node* node) {
    if (auto fairyContainer = dynamic_cast<FUIContainer*>(node)) {
        auto ownerComponent = dynamic_cast<GComponent*>(fairyContainer->gOwner);
        if (!ownerComponent)
            return;
//        ownerComponent->getChildren()
        ImGui::SetNextItemOpen(false, ImGuiCond_Once);
        if (ImGui::CollapsingHeader(_propertyType.c_str())) {
            proceedObject(ownerComponent);
        }
    }
}

void fairyProperty::viewPropertyFairyGUI(fairygui::GObject* object) {
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader(_propertyType.c_str())) {
        proceedObject(object);
    }
}

void fairyProperty::proceedObject(fairygui::GObject* object) {
    /*ImGui::SetNextItemOpen(false, ImGuiCond_Once);
    if (ImGui::CollapsingHeader(ax::StringUtils::format("Custom data (count:%zu)", object->getCustomData().size()).c_str())) {
        for (auto& item : object->getCustomData()) {
            if (item.second.getType() == ax::Value::Type::STRING)
                ImGui::Text("%s: %s ",item.first.c_str(), item.second.asString().c_str());
        }
    }

    //todo add relation viewer
    if (object->relations() && ImGui::CollapsingHeader("Relations")) {
        for (auto item : object->relations()->getRelationItems()) {
            for (auto relation : item->getRelations()) {
//                relation.type
            }
        }
//        object->relations().
//        for ()
    }*/
}
