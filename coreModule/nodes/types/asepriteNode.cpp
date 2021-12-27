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
        auto frames = object["frames"].GetObject();
        for (auto item = frames.MemberBegin(); item != frames.MemberEnd(); ++item) {
            if (item->value.IsObject()) {
                auto frame = item->value.GetObject();
//                frame
            }
        }
    }
    return false;
}

bool asepriteNode::hasAnimation(const std::string& name) {
    return false;
}

bool asepriteNode::setAnimation(const std::string& name) {
    return false;
}

bool asepriteNode::setAnimationFrames(const std::string& name, int firstFrame, int lastFrame) {
    return false;
}

bool asepriteNode::setAnimationFrame(const std::string& name, int frame) {
    return false;
}
