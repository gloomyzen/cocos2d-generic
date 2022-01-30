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
            sAnimFrame() = default;
            float duration;
            std::string spriteFrameId; //uuid
            bool load(const jsonObject& data, const std::string& fullPath, const std::string& cacheId);
        };

        asepriteNode();
        ~asepriteNode() override;
        CREATE_FUNC(asepriteNode);
        /// Loading node with external list of animation
        bool load(const jsonObject& object, const jsonObject& animations, const std::string& path);
        /// Loading node with animations from 'frameTags'
        bool load(const jsonObject& object, const std::string& path);
        bool hasAnimation(const std::string& name);
        bool setAnimation(const std::string& name, bool loop = false);
        void setUsePixelMode(bool value) override;
    private:
        bool loadFrames(const jsonObject& object, const std::map<std::string, std::pair<int, int>>& anim, const std::string& fullPath);
        std::map<std::string, std::vector<std::shared_ptr<sAnimFrame>>> animationsMap;
        std::string animation;
        bool usePixel = false;
    };
}// namespace generic::coreModule


#endif// GENERIC_ASEPRITENODE_H
