#include "spineProperty.h"
#include "generic/utilityModule/logManager.h"
#include "generic/utilityModule/stringUtility.h"
#include "spine/spine-cocos2dx.h"
#include "ui/CocosGUI.h"
#include <fmt/core.h>

using namespace generic::coreModule;


void spineProperty::parseProperty(ax::Node* node, const jsonObject& object) {
    if (auto spine = dynamic_cast<spine::SkeletonAnimation*>(node)) {
        auto scale = 1.f;
        if (object.HasMember("scale") && object["scale"].IsNumber()) {
            scale = object["scale"].GetFloat();
        }
        if (object.HasMember("file") && object["file"].IsString()) {
            auto atlas = fmt::format("{}.atlas", object["file"].GetString());
            auto skel = fmt::format("{}.skel", object["file"].GetString());
            auto json = fmt::format("{}.json", object["file"].GetString());
            if (ax::FileUtils::getInstance()->isFileExist(atlas) && ax::FileUtils::getInstance()->isFileExist(skel)) {
                spine->initWithBinaryFile(skel, atlas, scale);
                spine->autorelease();
            } else if (ax::FileUtils::getInstance()->isFileExist(atlas) && ax::FileUtils::getInstance()->isFileExist(json)) {
                spine->initWithJsonFile(json, atlas, scale);
                spine->autorelease();
            } else {
                LOG_ERROR("Can't get atlas or binary file for spine '{}'", node->getName().data());
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
        LOG_ERROR("Node '{}' no has spine property", propertyName.c_str());
    }
}

