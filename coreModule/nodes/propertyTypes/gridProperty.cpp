#include "gridProperty.h"
#include "generic/debugModule/logManager.h"
#include "generic/utilityModule/stringUtility.h"
#include <generic/coreModule/nodes/types/gridNode.h>

using namespace generic::coreModule;


void gridProperty::parseProperty(cocos2d::Node* node, const jsonObject& object) {
    if (auto grid = dynamic_cast<gridNode*>(node)) {
        if (object.HasMember("marginX") && object["marginX"].IsArray()) {
            auto array = object["marginX"].GetArray();
            if (array.Size() == 2u && array[0].IsInt() && array[1].IsInt()) {
                grid->setMarginX(array[0].GetInt(), array[1].GetInt());
            }
        }
        if (object.HasMember("marginY") && object["marginY"].IsArray()) {
            auto array = object["marginY"].GetArray();
            if (array.Size() == 2u && array[0].IsInt() && array[1].IsInt()) {
                grid->setMarginY(array[0].GetInt(), array[1].GetInt());
            }
        }
        if (object.HasMember("paddingX") && object["paddingX"].IsArray()) {
            auto array = object["paddingX"].GetArray();
            if (array.Size() == 2u && array[0].IsInt() && array[1].IsInt()) {
                grid->setPaddingX(array[0].GetInt(), array[1].GetInt());
            }
        }
        if (object.HasMember("paddingY") && object["paddingY"].IsArray()) {
            auto array = object["paddingY"].GetArray();
            if (array.Size() == 2u && array[0].IsInt() && array[1].IsInt()) {
                grid->setPaddingY(array[0].GetInt(), array[1].GetInt());
            }
        }
        if (object.HasMember("col") && object["col"].IsInt()) {
            grid->setColumns(object["col"].GetInt());
        }
        if (object.HasMember("row") && object["row"].IsInt()) {
            grid->setRows(object["row"].GetInt());
        }
        if (object.HasMember("direction") && object["direction"].IsString()) {
            auto direction = gridNode::getGridDirectionByString(object["direction"].GetString());
            grid->setDirection(direction);
        }
    } else {
        LOG_ERROR(CSTRING_FORMAT("Node '%s' no has grid property!", propertyName.c_str()));
    }
}

