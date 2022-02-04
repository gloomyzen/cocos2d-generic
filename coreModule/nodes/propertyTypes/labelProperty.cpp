#include "labelProperty.h"
#include "generic/debugModule/logManager.h"
#include "generic/utilityModule/stringUtility.h"

using namespace generic::coreModule;

std::string labelProperty::defaultFont = "fonts/arial.ttf";
std::map<std::string, cocos2d::TextHAlignment> labelProperty::textHAlignmentTypes = {
    {"center", cocos2d::TextHAlignment::CENTER},
    {"left", cocos2d::TextHAlignment::LEFT},
    {"right", cocos2d::TextHAlignment::RIGHT}
};

void labelProperty::parseProperty(cocos2d::Node* node, const jsonObject& object) {
    if (auto label = dynamic_cast<cocos2d::Label*>(node)) {
        if (object.HasMember("fontType") && object["fontType"].IsString() && object["fontType"].GetString() == std::string("ttf")) {
            cocos2d::TTFConfig font = label->getTTFConfig();
            if (font.fontFilePath.empty()) {
                font.fontFilePath = defaultFont;
            }
            font.outlineSize = 0;

            if (object.HasMember("fontSize") && object["fontSize"].IsNumber()) {
                font.fontSize = object["fontSize"].GetFloat();
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
            if (object.HasMember("align") && object["align"].IsString()) {
                cocos2d::TextHAlignment hAlignment = label->getTextAlignment();
                auto align = object["align"].GetString();
                if (textHAlignmentTypes.count(align)) {
                    hAlignment = textHAlignmentTypes.at(align);
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
        LOG_ERROR(CSTRING_FORMAT("Node '%s' no has label property!", propertyName.c_str()));
    }
}
