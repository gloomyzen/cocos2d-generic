#include "asepriteProperty.h"
#include "generic/coreModule/nodes/types/asepriteNode.h"
#include "generic/coreModule/resources/resourceManager.h"
#include "generic/debugModule/logManager.h"
#include "generic/utilityModule/stringUtility.h"

using namespace generic::coreModule;


void asepriteProperty::parseProperty(cocos2d::Node* node, const jsonObject& object) {
    if (auto aNode = dynamic_cast<asepriteNode*>(node)) {
        if (object.HasMember("file") && object["file"].IsString()) {
            auto data = GET_JSON(object["file"].GetString());
            if (!data.HasParseError() && data.IsObject()) {
                if (object.HasMember("animations") && object["animations"].IsObject()) {
                    aNode->load(data.GetObject(), object["animations"].GetObject(), object["file"].GetString());
                }
                else if (data.HasMember("meta") && data["meta"].HasMember("frameTags")) {
                    aNode->load(data.GetObject(), object["file"].GetString());
                }
            }
        }
        bool isLoop = false;
        if (object.HasMember("loop") && object["loop"].IsBool()) {
            isLoop = object["loop"].GetBool();
        }
        if (object.HasMember("animation") && object["animation"].IsString()) {
            aNode->setAnimation(object["animation"].GetString(), isLoop);
        }
        if (object.HasMember("usePixelMode") && object["usePixelMode"].IsBool()) {
            aNode->setUsePixelMode(object["usePixelMode"].GetBool());
        }
    } else {
        LOG_ERROR(CSTRING_FORMAT("Node '%s' no has aseprite property!", propertyName.c_str()));
    }
}
