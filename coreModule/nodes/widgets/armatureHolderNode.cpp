#include "armatureHolderNode.h"
#include "common/debugModule/logManager.h"
#include <map>

using namespace common::coreModule;

static std::map<armatureHolderNode::eArmatureState, const char*> statesMap = {
    { armatureHolderNode::eArmatureState::START, dragonBones::EventObject::START },
    { armatureHolderNode::eArmatureState::LOOP_COMPLETE, dragonBones::EventObject::LOOP_COMPLETE },
    { armatureHolderNode::eArmatureState::COMPLETE, dragonBones::EventObject::COMPLETE },
    { armatureHolderNode::eArmatureState::FADE_IN, dragonBones::EventObject::FADE_IN },
    { armatureHolderNode::eArmatureState::FADE_IN_COMPLETE, dragonBones::EventObject::FADE_IN_COMPLETE },
    { armatureHolderNode::eArmatureState::FADE_OUT, dragonBones::EventObject::FADE_OUT },
    { armatureHolderNode::eArmatureState::FADE_OUT_COMPLETE, dragonBones::EventObject::FADE_OUT_COMPLETE },
    { armatureHolderNode::eArmatureState::FRAME_EVENT, dragonBones::EventObject::FRAME_EVENT },
    { armatureHolderNode::eArmatureState::SOUND_EVENT, dragonBones::EventObject::SOUND_EVENT }
};

void armatureHolderNode::setDebug(bool value) {
    Node::setDebug(value);
    for (auto item : getChildren()) { item->setDebug(value); }
}

dragonBones::CCArmatureDisplay* armatureHolderNode::getArmatureNode() {
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

void armatureHolderNode::setAnimation(const std::string& name, std::function<void(cocos2d::EventCustom*)> clb) {
    if (auto node = getArmatureNode()) {
        node->getAnimation()->fadeIn(name);
    } else {
        LOG_ERROR(STRING_FORMAT("armatureHolderNode::setAnimation: animation %s not found!", name.c_str()));
    }
}

void armatureHolderNode::addChild(cocos2d::Node* child) {
    Node::addChild(child);
    if (auto armature = dynamic_cast<dragonBones::CCArmatureDisplay*>(child)) {
        boneNode = armature;
    }
}

void armatureHolderNode::addChild(cocos2d::Node* child, int localZOrder) {
    Node::addChild(child, localZOrder);
    if (auto armature = dynamic_cast<dragonBones::CCArmatureDisplay*>(child)) {
        boneNode = armature;
    }
}

void armatureHolderNode::addChild(cocos2d::Node* child, int localZOrder, int tag) {
    Node::addChild(child, localZOrder, tag);
    if (auto armature = dynamic_cast<dragonBones::CCArmatureDisplay*>(child)) {
        boneNode = armature;
    }
}

void armatureHolderNode::addChild(cocos2d::Node* child, int localZOrder, const std::string& name) {
    Node::addChild(child, localZOrder, name);
    if (auto armature = dynamic_cast<dragonBones::CCArmatureDisplay*>(child)) {
        boneNode = armature;
    }
}

/*
 * Example usage:
 * node->setAnimationCallback(armatureHolderNode::eArmatureState::COMPLETE, [](auto){});
 */
void armatureHolderNode::setAnimationCallback(armatureHolderNode::eArmatureState state, std::function<void(cocos2d::EventCustom*)> clb) {
    if (auto node = getArmatureNode()) {
        if (auto eventType = getEventType(state)) {
            setEventsEnabled(true);
            node->getEventDispatcher()->addCustomEventListener(eventType, clb);
        }
    } else {
        //todo log
    }
}

void armatureHolderNode::removeAnimationCallback(armatureHolderNode::eArmatureState state) {
    if (auto node = getArmatureNode()) {
        if (auto eventType = getEventType(state)) {
            node->getEventDispatcher()->removeCustomEventListeners(eventType);
        }
    } else {
        //todo log
    }
}

bool armatureHolderNode::isEventsEnable() const { return handleEvents; }

void armatureHolderNode::setEventsEnabled(bool value) {
    if (auto node = getArmatureNode()) {
        node->getEventDispatcher()->setEnabled(value);
        handleEvents = value;
    } else {
        //todo log
    }
}

const char* armatureHolderNode::getEventType(armatureHolderNode::eArmatureState state) {
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
void armatureHolderNode::setCustomAnimationCallback(const std::string& eventName, const std::function<void(cocos2d::EventCustom*)>& clb) {
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
        //todo log
    }
}

void armatureHolderNode::removeCustomAnimationCallback(const std::string& eventName) {
    auto find = customCallbacksMap.find(eventName);
    if (find != customCallbacksMap.end()) {
        customCallbacksMap.erase(find);
    }
}