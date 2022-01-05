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
            std::string spriteFrameId; //uuid
            bool load(const jsonObject& data, const std::string& fullPath);
        };

        asepriteNode() = default;
        ~asepriteNode() override;
        CREATE_FUNC(asepriteNode);
        bool load(const jsonObject& object, const jsonObject& animations, const std::string& path);
        bool hasAnimation(const std::string& name);
        bool setAnimation(const std::string& name, bool loop = false);
        void update(float delta) override;
    private:
        void animProceed(float delta = 0.f);
        std::map<std::string, std::vector<std::shared_ptr<sAnimFrame>>> animationsMap;
        struct sFrameOptions {
            std::string animation;
            bool loop = false;
            size_t index = 0u;
            unsigned millis = 0u;
        };
        sFrameOptions frame;
    };
}// namespace generic::coreModule


#endif// GENERIC_ASEPRITENODE_H
