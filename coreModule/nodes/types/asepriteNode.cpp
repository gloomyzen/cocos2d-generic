#include "asepriteNode.h"
#include <tuple>

using namespace generic::coreModule;

bool asepriteNode::load(const jsonObject& object, const jsonObject& animations) {
    if (object.HasMember("frames") && animations.MemberCount() != 0u) {
        std::map<std::string, std::pair<int, int>> anim;
        for (auto item = animations.MemberBegin(); item != animations.MemberEnd(); ++item) {
            if (item->value.IsArray() && item->value.GetArray().Size() == 2u) {
                auto array = item->value.GetArray();
                anim[item->name.GetString()] = std::make_pair(array[0u].GetInt(), array[1u].GetInt());
            }
        }
        //todo add loading from array export
        if (object["frames"].IsObject()) {
            auto frames = object["frames"].GetObject();
            auto pos = 0u;
            std::vector<std::string> memberNames;
            for (auto item = frames.MemberBegin(); item != frames.MemberEnd(); ++item) {
                memberNames.emplace_back(item->name.GetString());
            }
            for (auto [animName, animIndexes] : anim) {
                for (auto i = animIndexes.first; i < animIndexes.second; ++i) {
                    auto frame = std::make_shared<sAnimFrame>();
                    if (frame->load(frames[memberNames[static_cast<size_t>(i)].c_str()].GetObject())) {
                        if (animationsMap.count(animName) != 0u) {
                            animationsMap[animName].emplace_back(frame);
                        } else {
                            animationsMap[animName] = {frame};
                        }
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
        currentAnimation = name;
        currentAnimLoop = loop;
        return true;
    }
    return false;
}

bool asepriteNode::sAnimFrame::load(const jsonObject& data) {
    if (data.HasMember("frame") && data["frame"].IsObject()) {
        frameSize = cocos2d::Size(data["frame"]["w"].GetFloat(), data["frame"]["h"].GetFloat());
        framePos = cocos2d::Vec2(data["frame"]["x"].GetFloat(), data["frame"]["y"].GetFloat());
    } else {
        return false;
    }
    if (data.HasMember("rotated") && data["rotated"].IsBool()) {
        rotated = data["rotated"].GetBool();
    }
    if (data.HasMember("sourceSize") && data["sourceSize"].IsObject()) {
        sourceSize = cocos2d::Size(data["sourceSize"]["w"].GetFloat(), data["sourceSize"]["h"].GetFloat());
    } else {
        return false;
    }
    if (data.HasMember("duration") && data["duration"].IsNumber()) {
        duration = data["duration"].GetFloat() / 1000;
    } else {
        return false;
    }
    return true;
}
