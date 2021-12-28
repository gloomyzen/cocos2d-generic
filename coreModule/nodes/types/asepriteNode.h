#ifndef GENERIC_ASEPRITENODE_H
#define GENERIC_ASEPRITENODE_H

#include "cocos2d.h"
#include "generic/utilityModule/jsonHelper.h"
#include <functional>
#include <utility>
#include <map>
#include <string>
#include <vector>

namespace generic::coreModule {

    class asepriteNode : public cocos2d::Sprite {
    public:
        struct sAnimFrame {
            sAnimFrame() {}
            cocos2d::Size frameSize;
            cocos2d::Vec2 framePos;
            bool rotated = false;
            cocos2d::Size sourceSize;
            float duration;
            bool load(const jsonObject& data);
        };

        asepriteNode() = default;
        ~asepriteNode() override = default;
        CREATE_FUNC(asepriteNode);
        bool load(const jsonObject& object, const jsonObject& animations);
        bool hasAnimation(const std::string& name);
        bool setAnimation(const std::string& name, bool loop = false);
    private:
        std::map<std::string, std::vector<std::shared_ptr<sAnimFrame>>> animationsMap;
        std::string currentAnimation;
        bool currentAnimLoop = false;
    };
}// namespace generic::coreModule


#endif// GENERIC_ASEPRITENODE_H
