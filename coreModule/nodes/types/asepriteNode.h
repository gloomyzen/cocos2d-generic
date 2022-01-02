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
            float duration;
            std::string spriteFrameName; //uuid
            bool load(const jsonObject& data, const std::string& fullPath);
        };

        asepriteNode() = default;
        ~asepriteNode() override;
        CREATE_FUNC(asepriteNode);
        bool load(const jsonObject& object, const jsonObject& animations, const std::string& path);
        bool hasAnimation(const std::string& name);
        bool setAnimation(const std::string& name, bool loop = false);
    private:
        std::map<std::string, std::vector<std::shared_ptr<sAnimFrame>>> animationsMap;
        std::string currentAnimation;
        bool currentAnimLoop = false;
    };
}// namespace generic::coreModule


#endif// GENERIC_ASEPRITENODE_H
