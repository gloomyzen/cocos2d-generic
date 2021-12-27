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
            cocos2d::Size frameSize;
            cocos2d::Vec2 framePos;
            bool rotated = false;
            cocos2d::Size sourceSize;
            float duration;
        };

        asepriteNode() = default;
        ~asepriteNode() override = default;
        CREATE_FUNC(asepriteNode);
        bool load(const jsonObject& object);
        bool hasAnimation(const std::string& name);
        bool setAnimation(const std::string& name);
        bool setAnimationFrames(const std::string& name, int firstFrame, int lastFrame);
        bool setAnimationFrame(const std::string& name, int frame);
    private:
        std::map<std::string, std::vector<std::shared_ptr<sAnimFrame>>> animations;
    };
}// namespace generic::coreModule


#endif// GENERIC_ASEPRITENODE_H
