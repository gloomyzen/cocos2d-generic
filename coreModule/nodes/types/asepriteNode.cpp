#include "asepriteNode.h"
#include "generic/coreModule/resources/resourceManager.h"
#include "generic/utilityModule/uuidGenerator/uuidGenerator.h"
#include <tuple>

using namespace generic::coreModule;

bool asepriteNode::load(const jsonObject& object, const jsonObject& animations, const std::string& path) {
    if (object.HasMember("frames") && animations.MemberCount() != 0u) {
        std::map<std::string, std::pair<int, int>> anim;
        for (auto item = animations.MemberBegin(); item != animations.MemberEnd(); ++item) {
            if (item->value.IsArray() && item->value.GetArray().Size() == 2u) {
                auto array = item->value.GetArray();
                anim[item->name.GetString()] = std::make_pair(array[0u].GetInt(), array[1u].GetInt());
            }
        }
        auto fullPath = GET_RESOURCE_MANAGER().getImagePathWithExtension(path);
        if (fullPath.empty()) {
            return false;
        }
        //todo add loading from array export
        if (object["frames"].IsObject()) {
            auto frames = object["frames"].GetObject();
            std::vector<std::string> memberNames;
            for (auto item = frames.MemberBegin(); item != frames.MemberEnd(); ++item) {
                memberNames.emplace_back(item->name.GetString());
            }
            for (auto [animName, animIndexes] : anim) {
                for (auto i = animIndexes.first; i < animIndexes.second; ++i) {
                    auto frame = std::make_shared<sAnimFrame>();
                    if (frame->load(frames[memberNames[static_cast<size_t>(i)].c_str()].GetObject(), fullPath)) {
                        if (animationsMap.count(animName) != 0u) {
                            animationsMap[animName].emplace_back(frame);
                        } else {
                            animationsMap[animName] = {frame};
                        }
                    }
                }

            }
        }
//        initWithSpriteFrame() todo use this for anim switching
//        cocos2d::SpriteFrameCache::getInstance()->addSpriteFrame()
//        cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName()
//        initWithTextureFilename(fullPath, sourceSize);
        return true;
    }
    return false;
}

bool asepriteNode::hasAnimation(const std::string& name) {
    return animationsMap.count(name) != 0u;
}

bool asepriteNode::setAnimation(const std::string& name, bool loop) {
    if (hasAnimation(name)) {
        currentAnimation = name;
        currentAnimLoop = loop;
        return true;
    }
    return false;
}

asepriteNode::~asepriteNode() {
    //todo clear cache
}

bool asepriteNode::sAnimFrame::load(const jsonObject& data, const std::string& fullPath) {
    if (!data.HasMember("frame") || !data["frame"].IsObject()) {
        return false;
    }
    auto frameRect = cocos2d::Size(data["frame"]["w"].GetFloat(), data["frame"]["h"].GetFloat());
    auto frameOffset = cocos2d::Vec2(data["frame"]["x"].GetFloat(), data["frame"]["y"].GetFloat());
    auto rotated = false;
    if (data.HasMember("rotated") && data["rotated"].IsBool()) {
        rotated = data["rotated"].GetBool();
    }
    if (data.HasMember("duration") && data["duration"].IsNumber()) {
        duration = data["duration"].GetFloat() / 1000;
    } else {
        return false;
    }
    spriteFrameName = GET_UUID_GENERATOR().getRandom();
    auto spriteFrame = cocos2d::SpriteFrame::create(fullPath, cocos2d::Rect(frameOffset, frameRect), rotated, frameOffset, frameRect);
    cocos2d::SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, spriteFrameName);
    return true;
}
