#include "scrollViewProperty.h"
#include "generic/debugModule/logManager.h"
#include "generic/utilityModule/stringUtility.h"
#include "ui/CocosGUI.h"

using namespace generic::coreModule;

std::map<std::string, cocos2d::ui::ScrollView::Direction> scrollViewProperty::scrollDirectionsMap = {
    {"none", cocos2d::ui::ScrollView::Direction::NONE},
    {"horizontal", cocos2d::ui::ScrollView::Direction::HORIZONTAL},
    {"vertical", cocos2d::ui::ScrollView::Direction::VERTICAL},
    {"both", cocos2d::ui::ScrollView::Direction::BOTH}
};

void scrollViewProperty::parseProperty(cocos2d::Node* node, const jsonObject& object) {
    if (auto scrollNode = dynamic_cast<cocos2d::ui::ScrollView*>(node)) {
        // direction
        auto direction = cocos2d::ui::ScrollView::Direction::NONE;
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
        if (object.HasMember("bounceOffset") && object["bounceOffset"].IsArray()) {
            auto tempArray = object["bounceOffset"].GetArray();
            if (tempArray[0].IsNumber() && tempArray[1].IsNumber()) {
                scrollNode->setBounceOffset(tempArray[0].GetFloat(), tempArray[1].GetFloat());
            }
        }
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
                cocos2d::Size innerSize;
                innerSize.width = containerSize[0].GetFloat();
                innerSize.height = containerSize[1].GetFloat();
                scrollNode->setInnerContainerSize(innerSize);
            }
        }
        // start scroll position
        if (object.HasMember("scrollPosPercent") && object["scrollPosPercent"].IsArray()) {
            auto scrollPosPercent = object["scrollPosPercent"].GetArray();
            if (scrollPosPercent.Size() == 2u) {
                cocos2d::Vec2 jumpPos;
                jumpPos.x = scrollPosPercent[0].GetFloat();
                jumpPos.y = scrollPosPercent[1].GetFloat();
                switch (direction) {
                case cocos2d::ui::ScrollView::Direction::VERTICAL:
                    scrollNode->jumpToPercentVertical(jumpPos.y);
                    break;
                case cocos2d::ui::ScrollView::Direction::HORIZONTAL:
                    scrollNode->jumpToPercentHorizontal(jumpPos.x);
                    break;
                case cocos2d::ui::ScrollView::Direction::BOTH:
                case cocos2d::ui::ScrollView::Direction::NONE:
                    scrollNode->jumpToPercentBothDirection(jumpPos);
                    break;
                }
            }
        }
    } else {
        LOG_ERROR(CSTRING_FORMAT("Node '%s' no has scrollView property!", propertyName.c_str()));
    }
}

