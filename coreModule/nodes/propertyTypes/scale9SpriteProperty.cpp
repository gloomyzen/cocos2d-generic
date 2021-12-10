#include "scale9SpriteProperty.h"
#include "generic/debugModule/logManager.h"
#include "generic/utilityModule/stringUtility.h"
#include "ui/CocosGUI.h"

using namespace generic::coreModule;


void scale9SpriteProperty::parseProperty(cocos2d::Node* node, const jsonObject& object) {
    if (auto sprite = dynamic_cast<cocos2d::ui::Scale9Sprite*>(node)) {
        cocos2d::Rect sliceRect = cocos2d::Rect::ZERO;
        if (object.HasMember("image") && object["image"].IsString()) {
            std::string imagePath;
            imagePath = object["image"].GetString();
            if (imagePath.empty()) {
                LOG_ERROR(CSTRING_FORMAT("Property '%s' has invalid image path!", propertyName.c_str()));
                return;
            }
            sprite->initWithFile(imagePath);
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
            sprite->setCapInsets(sliceRect);
        }
        if (object.HasMember("size") && object["size"].IsArray()) {
            auto _size = cocos2d::Size();
            auto size = object["size"].GetArray();
            if (size.Size() == 2u) {
                _size.width = size[0].GetFloat();
                _size.height = size[1].GetFloat();
                sprite->setContentSize(_size);
            }
        }
    } else {
        LOG_ERROR(CSTRING_FORMAT("Node '%s' no has scale9Sprite property!", propertyName.c_str()));
    }
}