#include "armatureNode.h"
#include "common/debugModule/logManager.h"
#include "common/utilityModule/stringUtility.h"
#include <map>

using namespace common::coreModule;

static std::map<armatureNode::eArmatureState, const char*> statesMap = {
    { armatureNode::eArmatureState::START, dragonBones::EventObject::START },
    { armatureNode::eArmatureState::LOOP_COMPLETE, dragonBones::EventObject::LOOP_COMPLETE },
    { armatureNode::eArmatureState::COMPLETE, dragonBones::EventObject::COMPLETE },
    { armatureNode::eArmatureState::FADE_IN, dragonBones::EventObject::FADE_IN },
    { armatureNode::eArmatureState::FADE_IN_COMPLETE, dragonBones::EventObject::FADE_IN_COMPLETE },
    { armatureNode::eArmatureState::FADE_OUT, dragonBones::EventObject::FADE_OUT },
    { armatureNode::eArmatureState::FADE_OUT_COMPLETE, dragonBones::EventObject::FADE_OUT_COMPLETE },
    { armatureNode::eArmatureState::FRAME_EVENT, dragonBones::EventObject::FRAME_EVENT },
    { armatureNode::eArmatureState::SOUND_EVENT, dragonBones::EventObject::SOUND_EVENT }
};

//void armatureNode::setDebug(bool value) {
//    Node::setDebug(value);
//    for (auto item : getChildren()) { item->setDebug(value); }
//}

dragonBones::CCArmatureDisplay* armatureNode::getArmatureNode() {
    if (boneNode) {
        return boneNode;
    }
    for (auto item : getChildren()) {
        if (auto armature = dynamic_cast<dragonBones::CCArmatureDisplay*>(item)) {
            boneNode = armature;
            return boneNode;
        }
    }
    return nullptr;
}

void armatureNode::setAnimation(const std::string& name, std::function<void(cocos2d::EventCustom*)> clb) {
    if (auto node = getArmatureNode()) {
        node->getAnimation()->fadeIn(name);
    } else {
        LOG_ERROR(STRING_FORMAT("armatureNode::setAnimation: animation %s not found!", name.c_str()));
    }
}

void armatureNode::addChild(cocos2d::Node* child) {
    Node::addChild(child);
    if (auto armature = dynamic_cast<dragonBones::CCArmatureDisplay*>(child)) {
        boneNode = armature;
    }
}

void armatureNode::addChild(cocos2d::Node* child, int localZOrder) {
    Node::addChild(child, localZOrder);
    if (auto armature = dynamic_cast<dragonBones::CCArmatureDisplay*>(child)) {
        boneNode = armature;
    }
}

void armatureNode::addChild(cocos2d::Node* child, int localZOrder, int tag) {
    Node::addChild(child, localZOrder, tag);
    if (auto armature = dynamic_cast<dragonBones::CCArmatureDisplay*>(child)) {
        boneNode = armature;
    }
}

void armatureNode::addChild(cocos2d::Node* child, int localZOrder, const std::string& name) {
    Node::addChild(child, localZOrder, name);
    if (auto armature = dynamic_cast<dragonBones::CCArmatureDisplay*>(child)) {
        boneNode = armature;
    }
}

/*
 * Example usage:
 * node->setAnimationCallback(armatureNode::eArmatureState::COMPLETE, [](auto){});
 */
void armatureNode::setAnimationCallback(armatureNode::eArmatureState state, std::function<void(cocos2d::EventCustom*)> clb) {
    if (auto node = getArmatureNode()) {
        if (auto eventType = getEventType(state)) {
            setEventsEnabled(true);
            node->getEventDispatcher()->addCustomEventListener(eventType, clb);
        }
    } else {
        LOG_ERROR("armatureNode::setAnimationCallback: bones not found");
    }
}

void armatureNode::removeAnimationCallback(armatureNode::eArmatureState state) {
    if (auto node = getArmatureNode()) {
        if (auto eventType = getEventType(state)) {
            node->getEventDispatcher()->removeCustomEventListeners(eventType);
        }
    } else {
        LOG_ERROR("armatureNode::removeAnimationCallback: bones not found");
    }
}

bool armatureNode::isEventsEnable() const { return handleEvents; }

void armatureNode::setEventsEnabled(bool value) {
    if (auto node = getArmatureNode()) {
        node->getEventDispatcher()->setEnabled(value);
        handleEvents = value;
    } else {
        LOG_ERROR("armatureNode::setEventsEnabled: bones not found");
    }
}

const char* armatureNode::getEventType(armatureNode::eArmatureState state) {
    auto item = statesMap.find(state);
    if (item != statesMap.end()) {
        return item->second;
    }
    return nullptr;
}

/*
 * Example usage:
 * node->setCustomAnimationCallback("eventAttack", [](auto){});
 * "eventAttack" is custom event from dragonbones timeline
 */
void armatureNode::setCustomAnimationCallback(const std::string& eventName, const std::function<void(cocos2d::EventCustom*)>& clb) {
    if (auto node = getArmatureNode()) {
        setEventsEnabled(true);
        customCallbacksMap[eventName] = clb;
        node->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FRAME_EVENT, [this](cocos2d::EventCustom* event) {
               if (auto data = static_cast<dragonBones::EventObject*>(event->getUserData())) {
                   auto find = customCallbacksMap.find(data->name);
                   if (find != customCallbacksMap.end()) {
                       LOG_INFO(STRING_FORMAT("Node %s, founded event: %s, start callback", getName().c_str(), data->name.c_str()));
                       find->second(event);
                   }
               }
        });
    } else {
        LOG_ERROR("armatureNode::setCustomAnimationCallback: bones not found");
    }
}

void armatureNode::removeCustomAnimationCallback(const std::string& eventName) {
    auto find = customCallbacksMap.find(eventName);
    if (find != customCallbacksMap.end()) {
        customCallbacksMap.erase(find);
    }
}
