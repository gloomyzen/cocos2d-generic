#include "asepriteNode.h"
#include "generic/coreModule/resources/resourceManager.h"
#include <tuple>
#include <cmath>

using namespace generic::coreModule;

asepriteNode::asepriteNode() {}

asepriteNode::~asepriteNode() {
    for (const auto& [_, frames] : animationsMap) {
        for (const auto& item : frames) {
            cocos2d::SpriteFrameCache::getInstance()->removeSpriteFrameByName(item->spriteFrameId);
        }
    }
    animationsMap.clear();
}

bool asepriteNode::load(const jsonObject& object, const jsonObject& animations, const std::string& path) {
    auto fullPath = GET_RESOURCE_MANAGER().getImagePathWithExtension(path);
    if (fullPath.empty()) {
        return false;
    }
    if (animations.MemberCount() != 0u) {
        std::map<std::string, std::pair<int, int>> anim;
        for (auto item = animations.MemberBegin(); item != animations.MemberEnd(); ++item) {
            if (item->value.IsArray() && item->value.GetArray().Size() == 2u) {
                auto array = item->value.GetArray();
                anim[item->name.GetString()] = std::make_pair(array[0u].GetInt(), array[1u].GetInt());
            }
        }
        return loadFrames(object, anim, fullPath);
    }
    return false;
}

bool asepriteNode::load(const jsonObject& object, const std::string& path) {
    auto fullPath = GET_RESOURCE_MANAGER().getImagePathWithExtension(path);
    if (fullPath.empty()) {
        return false;
    }
    if (object.HasMember("meta") && object["meta"].HasMember("frameTags")) {
        auto tags = object["meta"]["frameTags"].GetArray();
        std::map<std::string, std::pair<int, int>> anim;
        for (const auto& obj : tags) {
            if (obj.IsObject()) {
                auto item = obj.GetObject();
                if (item.HasMember("name") && item.HasMember("from") && item.HasMember("to")) {
                    anim[item["name"].GetString()] = std::make_pair(item["from"].GetInt(), item["to"].GetInt());
                }
            }
        }
        return loadFrames(object, anim, fullPath);
    }
    return false;
}

bool asepriteNode::loadFrames(const jsonObject& object,
                              const std::map<std::string, std::pair<int, int>>& anim,
                              const std::string& fullPath) {
    //parsing object/hash-type aseprite import
    if (object.HasMember("frames") && object["frames"].IsObject()) {
        auto frames = object["frames"].GetObject();
        std::vector<std::string> memberNames;
        for (auto item = frames.MemberBegin(); item != frames.MemberEnd(); ++item) {
            memberNames.emplace_back(item->name.GetString());
        }
        for (auto [animName, animIndexes] : anim) {
            for (auto i = animIndexes.first; i <= animIndexes.second; ++i) {
                auto framePtr = std::make_shared<sAnimFrame>();
                auto cacheId = cocos2d::StringUtils::format("%d_%s", this->_ID, memberNames[static_cast<size_t>(i)].c_str());
                if (framePtr->load(frames[memberNames[static_cast<size_t>(i)].c_str()].GetObject(), fullPath, cacheId)) {
                    if (animationsMap.count(animName) != 0u) {
                        animationsMap[animName].emplace_back(framePtr);
                    } else {
                        animationsMap[animName] = {framePtr};
                    }
                }
            }

        }
        if (animation.empty() && !animationsMap.empty()) {
            setAnimation(animationsMap.begin()->first);
        }
        return true;
    }
    //parsing array-type aseprite import
    else if (object.HasMember("frames") && object["frames"].IsArray()) {
        auto frames = object["frames"].GetArray();
        for (auto [animName, animIndexes] : anim) {
            for (auto i = animIndexes.first; i <= animIndexes.second; ++i) {
                auto obj = frames[static_cast<unsigned>(i)].GetObject();
                auto framePtr = std::make_shared<sAnimFrame>();
                auto cacheId = cocos2d::StringUtils::format("%d_%s", this->_ID, obj["filename"].GetString());
                if (framePtr->load(obj, fullPath, cacheId)) {
                    if (animationsMap.count(animName) != 0u) {
                        animationsMap[animName].emplace_back(framePtr);
                    } else {
                        animationsMap[animName] = {framePtr};
                    }
                }
            }

        }
        return true;
    }
    return false;
}

bool asepriteNode::hasAnimation(const std::string& name) {
    return animationsMap.count(name) != 0u;
}

bool asepriteNode::setAnimation(const std::string& name, bool loop) {
    if (hasAnimation(name)) {
        animation = name;
        const auto& animations = animationsMap[animation];
        initWithSpriteFrameName(animations[0u]->spriteFrameId);
        cocos2d::Vector<cocos2d::AnimationFrame*> list;
        float allDuration = 0.f;
        for (const auto& item : animations) {
            if (auto spriteFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(item->spriteFrameId)) {
                auto frame = cocos2d::AnimationFrame::create(spriteFrame, item->duration, {});
                allDuration += item->duration;
                list.pushBack(frame);
            }
        }
        auto anim = cocos2d::Animation::create(list, allDuration);
        auto animAction = cocos2d::Animate::create(anim);
        if (loop) {
            runAction(cocos2d::RepeatForever::create(animAction));
        } else {
            runAction(animAction);
        }

        return true;
    }
    return false;
}

void asepriteNode::setUsePixelMode(bool value) {
    if (usePixel != value) {
        usePixel = value;
        for (const auto& [_, frames] : animationsMap) {
            for (const auto& item : frames) {
                if (auto spriteFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(item->spriteFrameId)) {
                    if (value)
                        spriteFrame->getTexture()->setAliasTexParameters();
                    else
                        spriteFrame->getTexture()->setAntiAliasTexParameters();
                }
            }
        }
    }
    Sprite::setUsePixelMode(value);
}

bool asepriteNode::sAnimFrame::load(const jsonObject& data, const std::string& fullPath, const std::string& cacheId) {
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
    spriteFrameId = cacheId;
    auto spriteFrame = cocos2d::SpriteFrame::create(fullPath, cocos2d::Rect(frameOffset, frameRect), rotated, cocos2d::Vec2::ZERO, frameRect);
    cocos2d::SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, spriteFrameId);
    return true;
}
