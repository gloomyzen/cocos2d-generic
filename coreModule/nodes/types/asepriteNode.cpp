#include "asepriteNode.h"

using namespace generic::coreModule;

bool asepriteNode::load(const jsonObject& object) {
    if (object.HasMember("frames")) {
        auto frames = object["frames"].GetObject();
        for (auto item = frames.MemberBegin(); item != frames.MemberEnd(); ++item) {
            auto name = item->name.GetString();
            auto val = item->value.IsObject();
            auto test = ";";
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
