#include "spriteProperty.h"
#include "generic/utilityModule/logManager.h"
#include "generic/utilityModule/stringUtility.h"

using namespace generic::coreModule;

#ifdef CC_SPRITE_SET_RENDERMODE
std::map<std::string, cocos2d::Sprite::RenderMode> spriteProperty::renderModeMap = {
    {"quad", cocos2d::Sprite::RenderMode::QUAD},
    {"polygon", cocos2d::Sprite::RenderMode::POLYGON},
    {"slice9", cocos2d::Sprite::RenderMode::SLICE9},
    {"quad_batchnode", cocos2d::Sprite::RenderMode::QUAD_BATCHNODE}
};
#endif

void spriteProperty::parseProperty(ax::Node* node, const jsonObject& object) {
   /*if (auto sprite3d = dynamic_cast<ax::Sprite3D*>(node)) {
        if (object.HasMember("image") && object["image"].IsString()) {
            sprite3d->initWithFile(object["image"].GetString());
//            sprite3d->setForce2DQueue(true);//todo probably it is setForceDepthWrite
            sprite3d->setBlendFunc(ax::BlendFunc::ALPHA_PREMULTIPLIED);
        }
    } else*/ if (auto sprite = dynamic_cast<ax::Sprite*>(node)) {
        std::string imagePath;
        bool isPoly = true;
        if (object.HasMember("image") && object["image"].IsString()) {
            imagePath = object["image"].GetString();
            if (imagePath.empty()) {
                LOG_ERROR("Property '{}' has invalid image path", propertyName.c_str());
                return;
            }
            if (object.HasMember("polygon") && object["polygon"].IsBool()) {
                isPoly = object["polygon"].GetBool();
            }
#ifdef CC_SPRITE_SET_RENDERMODE
            if (object.HasMember("renderMode") && object["renderMode"].IsString()) {
                auto renderModeStr = object["renderMode"].GetString();
                if (renderModeMap.count(renderModeStr)) {
                    sprite->setRenderMode(renderModeMap[renderModeStr]);
                    isPoly = false;
                }
            }
#endif

            if (isPoly) {
                auto polygon = ax::AutoPolygon::generatePolygon(imagePath);
                sprite->initWithPolygon(polygon);
            } else {
                sprite->initWithFile(imagePath);
            }
            if (object.HasMember("alphaMode") && object["alphaMode"].IsNumber()) {
                switch (object["alphaMode"].GetInt()) {
                case 1: sprite->setBlendFunc(ax::BlendFunc::ADDITIVE);
                case 2: sprite->setBlendFunc(ax::BlendFunc::ALPHA_PREMULTIPLIED);
                case 3: sprite->setBlendFunc(ax::BlendFunc::ALPHA_NON_PREMULTIPLIED);
                case 4: sprite->setBlendFunc(ax::BlendFunc::DISABLE);
                default: sprite->setBlendFunc(ax::BlendFunc::ALPHA_PREMULTIPLIED); break;
                }
            }
        } else {
            LOG_ERROR("Property '{}' no has image path", propertyName.c_str());
            return;
        }
    } else {
        LOG_ERROR("Node '{}' no has sprite property", propertyName.c_str());
    }
}
