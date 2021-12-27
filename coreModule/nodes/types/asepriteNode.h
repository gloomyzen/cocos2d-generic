#ifndef GENERIC_ASEPRITENODE_H
#define GENERIC_ASEPRITENODE_H

#include "cocos2d.h"
#include "generic/utilityModule/jsonHelper.h"
#include <functional>
#include <utility>
#include <map>
#include <string>

namespace generic::coreModule {

    class asepriteNode : public cocos2d::Sprite {
    public:
        asepriteNode() = default;
        ~asepriteNode() override = default;
        CREATE_FUNC(asepriteNode);
        void load(const jsonObject& object);
        bool hasAnimation(const std::string& name);
        bool setAnimation(const std::string& name);
    private:
        std::map<std::string, std::string> animations;
    };
}// namespace generic::coreModule


#endif// GENERIC_ASEPRITENODE_H
