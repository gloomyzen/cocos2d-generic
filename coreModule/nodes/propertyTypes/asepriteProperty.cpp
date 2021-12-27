#include "asepriteProperty.h"
#include "generic/coreModule/nodes/types/asepriteNode.h"
#include "generic/coreModule/resources/resourceManager.h"
#include "generic/debugModule/logManager.h"
#include "generic/utilityModule/stringUtility.h"

using namespace generic::coreModule;


void asepriteProperty::parseProperty(cocos2d::Node* node, const jsonObject& object) {
    if (auto aNode = dynamic_cast<asepriteNode*>(node)) {
        if (object.HasMember("file") && object["file"].IsString()) {
            auto path = STRING_FORMAT("%s.json", object["file"].GetString());
            auto data = GET_JSON(path);
            if (data.IsObject()) {
                aNode->load(data.GetObject());
            }
        }
        if (object.HasMember("animations") && object["animations"].IsObject()) {
            //
        }
        if (object.HasMember("animation") && object["animation"].IsString()) {
            //
        }
        if (object.HasMember("loop") && object["loop"].IsBool()) {
            //
        }
    } else {
        LOG_ERROR(CSTRING_FORMAT("Node '%s' no has aseprite property!", propertyName.c_str()));
    }
}
