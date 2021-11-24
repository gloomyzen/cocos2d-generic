#include "labelProperty.h"
#include "generic/debugModule/logManager.h"
#include "generic/utilityModule/stringUtility.h"

using namespace generic::coreModule;


void labelProperty::parseProperty(cocos2d::Node* node, const jsonObject& object) {
    if (auto label = dynamic_cast<cocos2d::Label*>(node)) {
        if (object.HasMember("fontType") && object["fontType"].IsString() && object["fontType"].GetString() == std::string("ttf")) {
            cocos2d::TTFConfig font = label->getTTFConfig();
            if (font.fontFilePath.empty()) {
                font.fontFilePath = "fonts/arial.ttf";
            }
            font.outlineSize = 0;

            if (object.HasMember("fontSize") && object["fontSize"].IsNumber()) {
                font.fontSize = object["fontSize"].GetFloat() * 2;
                label->setScale(0.5f);
            }
            if (object.HasMember("fontFile") && object["fontFile"].IsString()) {
                font.fontFilePath = object["fontFile"].GetString();
            }
            if (object.HasMember("bold") && object["bold"].IsBool()) {
                font.bold = object["bold"].GetBool();
            }
            if (object.HasMember("italic") && object["italic"].IsBool()) {
                font.italics = object["italic"].GetBool();
            }
            if (object.HasMember("alight") && object["alight"].IsString()) {
                cocos2d::TextHAlignment hAlignment = label->getTextAlignment();
                auto alight = object["alight"].GetString();
                if (alight == std::string("center")) {
                    hAlignment = cocos2d::TextHAlignment::CENTER;
                } else if (alight == std::string("left")) {
                    hAlignment = cocos2d::TextHAlignment::LEFT;
                } else if (alight == std::string("right")) {
                    hAlignment = cocos2d::TextHAlignment::RIGHT;
                }
                label->setAlignment(hAlignment);
            }
            if (object.HasMember("maxLineWidth") && object["maxLineWidth"].IsNumber()) {
                label->setMaxLineWidth(object["maxLineWidth"].GetFloat());
            }
            label->setTTFConfig(font);
            if (object.HasMember("text") && object["text"].IsString()) {
                label->setString(object["text"].GetString());
            }
        }
    } else {
        LOG_ERROR(CSTRING_FORMAT("Property '%s' no has label node type!", propertyName.c_str()));
    }
}

