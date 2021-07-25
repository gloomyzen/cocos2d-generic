#include "buttonBase.h"

using namespace generic::coreModule;

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
            sprite->setLocalZOrder(0);
        }
    }
    sprite->initWithFile(image);
    sprite->setAnchorPoint(cocos2d::Vec2::ZERO);
    setContentSize(sprite->getContentSize());
}

void buttonBase::setButtonBgSprite(cocos2d::Sprite* pSprite) {
    std::int32_t pos = -1;
    if (sprite == nullptr) {
        if (auto node = dynamic_cast<cocos2d::Sprite*>(findNode("buttonNode"))) {
            pos = node->getLocalZOrder();
            node->removeFromParent();
        }
    } else {
        pos = sprite->getLocalZOrder();
        sprite->removeFromParent();
    }
    sprite = pSprite;
    sprite->setName("buttonNode");
    if (sprite->getParent() == nullptr) {
        addChild(sprite);
    }
    sprite->setLocalZOrder(pos);
}
