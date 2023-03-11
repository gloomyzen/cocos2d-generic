#include "scale9SpriteProperty.h"
#include "generic/utilityModule/logManager.h"
#include "generic/utilityModule/stringUtility.h"
#include "ui/CocosGUI.h"

using namespace generic::coreModule;


void scale9SpriteProperty::parseProperty(ax::Node* node, const jsonObject& object) {
    if (auto scaleSprite = dynamic_cast<ax::ui::Scale9Sprite*>(node)) {
        ax::Rect sliceRect = ax::Rect::ZERO;
        if (object.HasMember("image") && object["image"].IsString()) {
            std::string imagePath;
            imagePath = object["image"].GetString();
            if (imagePath.empty()) {
                LOG_ERROR(CSTRING_FORMAT("Property '%s' has invalid image path!", propertyName.c_str()));
                return;
            }
            scaleSprite->initWithFile(imagePath);
        }
        if (object.HasMember("slice9") && object["slice9"].IsObject()) {
            auto slice9 = object["slice9"].GetObject();
            if (slice9.HasMember("x") && slice9["x"].IsNumber()) {
                sliceRect.origin.x = slice9["x"].GetFloat();
            }
            if (slice9.HasMember("y") && slice9["y"].IsNumber()) {
                sliceRect.origin.y = slice9["y"].GetFloat();
            }
            if (slice9.HasMember("width") && slice9["width"].IsNumber()) {
                sliceRect.size.width = slice9["width"].GetFloat();
            }
            if (slice9.HasMember("height") && slice9["height"].IsNumber()) {
                sliceRect.size.height = slice9["height"].GetFloat();
            }
            scaleSprite->setCapInsets(sliceRect);
        }
        if (object.HasMember("size") && object["size"].IsArray()) {
            auto _size = ax::Size();
            auto size = object["size"].GetArray();
            if (size.Size() == 2u) {
                _size.width = size[0].GetFloat();
                _size.height = size[1].GetFloat();
                scaleSprite->setContentSize(_size);
            }
        }
    }
    else if (auto sprite = dynamic_cast<ax::Sprite*>(node)) {
        auto tempSprite = new ax::ui::Scale9Sprite();
        parseProperty(tempSprite, object);
        tempSprite->setAnchorPoint(ax::Vec2::ZERO);
        auto rTarget = ax::RenderTexture::create(tempSprite->getContentSize().width, tempSprite->getContentSize().height);
        rTarget->begin();
        tempSprite->visit();
        rTarget->end();
        sprite->initWithTexture(rTarget->getSprite()->getTexture());
        if (rTarget->getSprite() && rTarget->getSprite()->getTexture()) {
            sprite->initWithTexture(rTarget->getSprite()->getTexture());
        } else {
            LOG_ERROR(CSTRING_FORMAT("Can not get texture from render target."));
        }
        AX_SAFE_RETAIN(tempSprite);
        AX_SAFE_RETAIN(rTarget);
    }
    else {
        LOG_ERROR(CSTRING_FORMAT("Node '%s' no has scale9Sprite property!", propertyName.c_str()));
    }
}
