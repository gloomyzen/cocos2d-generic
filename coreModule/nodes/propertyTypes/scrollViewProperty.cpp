#include "scrollViewProperty.h"
#include "generic/utilityModule/logManager.h"
#include "generic/utilityModule/stringUtility.h"
#include "ui/CocosGUI.h"

using namespace generic::coreModule;

std::map<std::string, ax::ui::ScrollView::Direction> scrollViewProperty::scrollDirectionsMap = {
    {"none", ax::ui::ScrollView::Direction::NONE},
    {"horizontal", ax::ui::ScrollView::Direction::HORIZONTAL},
    {"vertical", ax::ui::ScrollView::Direction::VERTICAL},
    {"both", ax::ui::ScrollView::Direction::BOTH}
};

void scrollViewProperty::parseProperty(ax::Node* node, const jsonObject& object) {
    if (auto scrollNode = dynamic_cast<ax::ui::ScrollView*>(node)) {
        // direction
        auto direction = ax::ui::ScrollView::Direction::NONE;
        if (object.HasMember("direction") && object["direction"].IsString()) {
            std::string conf = object["direction"].GetString();
            if (scrollDirectionsMap.count(conf)) {
                direction = scrollDirectionsMap[conf];
            }
        }
        scrollNode->setDirection(direction);
        // bounce
        bool bounce = false;
        if (object.HasMember("bounce") && object["bounce"].IsBool()) {
            bounce = object["bounce"].GetBool();
        }
        scrollNode->setBounceEnabled(bounce);
//        if (object.HasMember("bounceOffset") && object["bounceOffset"].IsArray()) {
//            auto tempArray = object["bounceOffset"].GetArray();
//            if (tempArray[0].IsNumber() && tempArray[1].IsNumber()) {
//                scrollNode->setBounceOffset(tempArray[0].GetFloat(), tempArray[1].GetFloat());
//            }
//        }
        // scroll bar
        bool scrollBar = false;
        if (object.HasMember("scrollBar") && object["scrollBar"].IsBool()) {
            scrollBar = object["scrollBar"].GetBool();
        }
        scrollNode->setScrollBarEnabled(scrollBar);
        // inertial scroll (the best performance with horizontal or vertical)
        bool inertialScroll = false;
        if (object.HasMember("inertialScroll") && object["inertialScroll"].IsBool()) {
            inertialScroll = object["inertialScroll"].GetBool();
        }
        scrollNode->setInertiaScrollEnabled(inertialScroll);
        // inner container size
        if (object.HasMember("containerSize") && object["containerSize"].IsArray()) {
            auto containerSize = object["containerSize"].GetArray();
            if (containerSize.Size() == 2u) {
                ax::Size innerSize;
                innerSize.width = containerSize[0].GetFloat();
                innerSize.height = containerSize[1].GetFloat();
                scrollNode->setInnerContainerSize(innerSize);
            }
        }
        // start scroll position
        if (object.HasMember("scrollPosPercent") && object["scrollPosPercent"].IsArray()) {
            auto scrollPosPercent = object["scrollPosPercent"].GetArray();
            if (scrollPosPercent.Size() == 2u) {
                ax::Vec2 jumpPos;
                jumpPos.x = scrollPosPercent[0].GetFloat();
                jumpPos.y = scrollPosPercent[1].GetFloat();
                switch (direction) {
                case ax::ui::ScrollView::Direction::VERTICAL:
                    scrollNode->jumpToPercentVertical(jumpPos.y);
                    break;
                case ax::ui::ScrollView::Direction::HORIZONTAL:
                    scrollNode->jumpToPercentHorizontal(jumpPos.x);
                    break;
                case ax::ui::ScrollView::Direction::BOTH:
                case ax::ui::ScrollView::Direction::NONE:
                    scrollNode->jumpToPercentBothDirection(jumpPos);
                    break;
                }
            }
        }
    } else {
        LOG_ERROR(CSTRING_FORMAT("Node '%s' no has scrollView property!", propertyName.c_str()));
    }
}

