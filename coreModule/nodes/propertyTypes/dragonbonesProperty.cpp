#include "dragonbonesProperty.h"
#include "generic/coreModule/nodes/types/armatureNode.h"
#include "generic/utilityModule/logManager.h"
#include "generic/utilityModule/stringUtility.h"

using namespace generic::coreModule;


void dragonbonesProperty::parseProperty(ax::Node* node, const jsonObject& object) {
#if defined(AX_BUILD_WITH_DRANGBONES) && AX_BUILD_WITH_DRANGBONES
    if (auto rootNode = dynamic_cast<armatureNode*>(node)) {
        if (object.HasMember("texFile") && object.HasMember("skeFile")) {
            if (cachedArmature.find(object["name"].GetString()) == cachedArmature.end()) {
                // todo check file exists
                dragonBones::CCFactory::getFactory()->loadTextureAtlasData(object["texFile"].GetString());
                dragonBones::CCFactory::getFactory()->loadDragonBonesData(object["skeFile"].GetString());
                cachedArmature[object["name"].GetString()] = true;
            }
            auto bone = dragonBones::CCFactory::getFactory()->buildArmatureDisplay(object["name"].GetString());
            if (bone->getArmature()) {
                if (object.HasMember("frameRate") && object["frameRate"].IsNumber()) {
                    bone->getArmature()->setCacheFrameRate(object["frameRate"].GetInt());
                }
                int playTimes = -1;
                float fadeInTime = -1.f;
                if (object.HasMember("playTimes") && object["playTimes"].IsNumber()) {
                    playTimes = object["playTimes"].GetInt();
                }
                if (object.HasMember("fadeInTime") && object["fadeInTime"].IsNumber()) {
                    fadeInTime = object["fadeInTime"].GetFloat();
                }
                if (object.HasMember("animation") && object["animation"].IsString()) {
                    if (bone->getAnimation()->hasAnimation(object["animation"].GetString())) {
                        bone->getAnimation()->fadeIn(object["animation"].GetString(), fadeInTime, playTimes);
                    } else {
                        LOG_ERROR("Can't find animation '{}'", object["animation"].GetString());
                    }
                }
                rootNode->addChild(bone);
            } else {
                LOG_ERROR("Can't get any armature from factory");
            }
        }
    } else {
        LOG_ERROR("Node '{}' no has dragonBones property", propertyName.c_str());
    }
#else
    LOG_ERROR("Engine can't support dragonBones property");
#endif
}
