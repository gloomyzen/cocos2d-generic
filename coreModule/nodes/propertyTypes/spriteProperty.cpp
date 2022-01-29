#include "spriteProperty.h"
#include "generic/debugModule/logManager.h"
#include "generic/utilityModule/stringUtility.h"

using namespace generic::coreModule;


void spriteProperty::parseProperty(cocos2d::Node* node, const jsonObject& object) {
   if (auto sprite3d = dynamic_cast<cocos2d::Sprite3D*>(node)) {
        if (object.HasMember("image") && object["image"].IsString()) {
            sprite3d->initWithFile(object["image"].GetString());
            sprite3d->setForce2DQueue(true);
            sprite3d->setBlendFunc(cocos2d::BlendFunc::ALPHA_PREMULTIPLIED);
        }
    } else if (auto sprite = dynamic_cast<cocos2d::Sprite*>(node)) {
        std::string imagePath;
        bool isPoly = true;
        if (object.HasMember("image") && object["image"].IsString()) {
            imagePath = object["image"].GetString();
            if (imagePath.empty()) {
                LOG_ERROR(CSTRING_FORMAT("Property '%s' has invalid image path!", propertyName.c_str()));
                return;
            }
            if (object.HasMember("polygon") && object["polygon"].IsBool()) {
                isPoly = object["polygon"].GetBool();
            }

            if (isPoly) {
                auto polygon = cocos2d::AutoPolygon::generatePolygon(imagePath);
                sprite->initWithPolygon(polygon);
            } else {
                sprite->initWithFile(imagePath);
            }
            if (object.HasMember("alphaMode") && object["alphaMode"].IsNumber()) {
                switch (object["alphaMode"].GetInt()) {
                case 1: sprite->setBlendFunc(cocos2d::BlendFunc::ADDITIVE);
                case 2: sprite->setBlendFunc(cocos2d::BlendFunc::ALPHA_PREMULTIPLIED);
                case 3: sprite->setBlendFunc(cocos2d::BlendFunc::ALPHA_NON_PREMULTIPLIED);
                case 4: sprite->setBlendFunc(cocos2d::BlendFunc::DISABLE);
                default: sprite->setBlendFunc(cocos2d::BlendFunc::ALPHA_PREMULTIPLIED); break;
                }
            }
        } else {
            LOG_ERROR(CSTRING_FORMAT("Property '%s' no has image path!", propertyName.c_str()));
            return;
        }
    } else {
        LOG_ERROR(CSTRING_FORMAT("Node '%s' no has sprite property!", propertyName.c_str()));
    }
}
