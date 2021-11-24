#include "spineProperty.h"
#include "generic/debugModule/logManager.h"
#include "generic/utilityModule/stringUtility.h"
#include "spine/spine-cocos2dx.h"
#include "ui/CocosGUI.h"

using namespace generic::coreModule;


void spineProperty::parseProperty(cocos2d::Node* node, const jsonObject& object) {
    if (auto spine = dynamic_cast<spine::SkeletonAnimation*>(node)) {
        auto scale = 1.f;
        if (object.HasMember("scale") && object["scale"].IsNumber()) {
            scale = object["scale"].GetFloat();
        }
        if (object.HasMember("file") && object["file"].IsString()) {
            auto atlas = STRING_FORMAT("%s.atlas", object["file"].GetString());
            auto skel = STRING_FORMAT("%s.skel", object["file"].GetString());
            auto json = STRING_FORMAT("%s.json", object["file"].GetString());
            if (cocos2d::FileUtils::getInstance()->isFileExist(atlas) && cocos2d::FileUtils::getInstance()->isFileExist(skel)) {
                spine->initWithBinaryFile(skel, atlas, scale);
                spine->autorelease();
            } else if (cocos2d::FileUtils::getInstance()->isFileExist(atlas) && cocos2d::FileUtils::getInstance()->isFileExist(json)) {
                spine->initWithJsonFile(json, atlas, scale);
                spine->autorelease();
            } else {
                LOG_ERROR(CSTRING_FORMAT("Can't get atlas or binary file for spine '%s'!",
                                         node->getName().c_str()));
            }
        }
        auto loop = false;
        if (object.HasMember("loop") && object["loop"].IsBool()) {
            loop = object["loop"].GetBool();
        }
        if (object.HasMember("animation") && object["animation"].IsString()) {
            auto animation = object["animation"].GetString();
            spine->setAnimation(1, animation, loop);
        }
        if (object.HasMember("skin") && object["skin"].IsString()) {
            spine->setSkin(object["skin"].GetString());
        }
    } else {
        LOG_ERROR(CSTRING_FORMAT("Node '%s' no has spine property!", propertyName.c_str()));
    }
}

