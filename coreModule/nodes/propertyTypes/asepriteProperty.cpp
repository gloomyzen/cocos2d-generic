#include "asepriteProperty.h"
#include "generic/debugModule/logManager.h"
#include "generic/utilityModule/stringUtility.h"

using namespace generic::coreModule;


void asepriteProperty::parseProperty(cocos2d::Node* node, const jsonObject& object) {
    if (auto asepriteNode = dynamic_cast<asepriteNode*>(node)) {

    } else {
        LOG_ERROR(CSTRING_FORMAT("Node '%s' no has aseprite property!", propertyName.c_str()));
    }
}

