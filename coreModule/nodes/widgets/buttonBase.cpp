#include "buttonBase.h"

using namespace common::coreModule;

buttonBase::buttonBase() {}

buttonBase::~buttonBase() = default;

void buttonBase::loadTexture(const std::string& image) {
    if (sprite == nullptr) {
        if (auto node = dynamic_cast<cocos2d::Sprite*>(findNode("buttonNode"))) {
            sprite = node;
        } else {
            sprite = cocos2d::Sprite::create();
            sprite->setName("buttonNode");
            addChild(sprite);
        }
    }
    sprite->initWithFile(image);
    sprite->setAnchorPoint(cocos2d::Vec2::ZERO);
}

