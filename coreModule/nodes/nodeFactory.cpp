#include "nodeFactory.h"
#include "DragonBones/CCDragonBonesHeaders.h"
#include "generic/coreModule/nodes/types/armatureNode.h"
#include "generic/coreModule/nodes/types/buttonBase.h"
#include "generic/coreModule/nodes/types/buttonNode.h"
#include "generic/coreModule/nodes/types/gridNode.h"
#include "generic/coreModule/nodes/types/node3d.h"
#include "generic/coreModule/nodes/types/soundButton.h"
#include "generic/coreModule/nodes/types/drawNodeBase.h"
#include "generic/debugModule/logManager.h"
#include "generic/utilityModule/stringUtility.h"
#include "spine/spine-cocos2dx.h"
#include "ui/CocosGUI.h"
#include <map>
#include <string>
#include <utility>

using namespace generic::coreModule;
using namespace cocos2d;
using namespace dragonBones;

std::map<std::string, eNodeFactory> componentsMap = {
    { "transformComponent", eNodeFactory::TRANSFORM_COMPONENT }, { "spriteComponent", eNodeFactory::SPRITE_COMPONENT },
    { "labelComponent", eNodeFactory::LABEL_COMPONENT },         { "dragonbonesComponent", eNodeFactory::DRAGONBONES_COMPONENT },
    { "colorComponent", eNodeFactory::COLOR_COMPONENT },         { "scrollViewComponent", eNodeFactory::SCROLL_VIEW_COMPONENT },
    { "gridComponent", eNodeFactory::GRID_COMPONENT },           { "scale9spriteComponent", eNodeFactory::SCALE9SPRITE_COMPONENT },
    { "spineComponent", eNodeFactory::SPINE_COMPONENT },         { "clipComponent", eNodeFactory::CLIP_COMPONENT }
};

nodeFactory* currentNodeFactory = nullptr;

nodeFactory::nodeFactory() {
    /// Default node types
    if (!inited) {
        inited = true;
        /// Core types
        nodes["node"] = []() { return Node::create(); };
        nodes["sprite"] = []() { return Sprite::create(); };
        nodes["sprite3d"] = []() { return Sprite3D::create(); };
        nodes["label"] = []() { return Label::create(); };
        nodes["buttonNode"] = []() { return buttonNode::create(); };
        nodes["layout"] = []() { return ui::Layout::create(); };
        nodes["layer"] = []() { return Layer::create(); };
        nodes["clippingNode"] = []() { return ClippingNode::create(); };
        nodes["scale9sprite"] = []() { return ui::Scale9Sprite::create(); };
        /// External types, in generic
        nodes["dragonbones"] = []() { return new armatureNode(); };
        nodes["spine"] = []() { return new spine::SkeletonAnimation(); };
        nodes["scrollView"] = []() { return ui::ScrollView::create(); };
        nodes["soundButton"] = []() { return soundButton::create(); };
        nodes["grid"] = []() { return gridNode::create(); };
        nodes["node3d"] = []() { return node3d::create(); };
    }
}

nodeFactory::~nodeFactory() = default;

void nodeFactory::cleanup() {
    nodes.clear();
    delete currentNodeFactory;
    currentNodeFactory = nullptr;
}

nodeFactory& nodeFactory::getInstance() {
    if (currentNodeFactory == nullptr) {
        currentNodeFactory = new nodeFactory();
    }
    return *currentNodeFactory;
}

void nodeFactory::readComponent(Node* node,
                                const std::string& componentName,
                                const rapidjson::GenericValue<rapidjson::UTF8<char>>::Object& object) {
    if (node == nullptr)
        return;

    if (!hasRegisteredComponent(componentName)) {
        LOG_ERROR(CSTRING_FORMAT("Component '%s' was not found!", componentName.c_str()));
        return;
    }

    eNodeFactory needle = componentsMap[componentName];

    node->setCascadeOpacityEnabled(true);
    node->setCascadeColorEnabled(true);

    switch (needle) {
    case eNodeFactory::TRANSFORM_COMPONENT: {

    } break;
    case eNodeFactory::SPRITE_COMPONENT: {

    } break;
    case eNodeFactory::LABEL_COMPONENT: {

    } break;
    case eNodeFactory::DRAGONBONES_COMPONENT: {

    } break;
    case eNodeFactory::SPINE_COMPONENT: {

    } break;
    case eNodeFactory::COLOR_COMPONENT: {

    } break;
    case eNodeFactory::SCROLL_VIEW_COMPONENT: {
        //
    }
    case eNodeFactory::GRID_COMPONENT: {

    } break;
    case eNodeFactory::SCALE9SPRITE_COMPONENT: {
        //
    } break;
    case eNodeFactory::CLIP_COMPONENT: {

    } break;
    }
}

bool nodeFactory::hasRegisteredComponent(const std::string& componentName) {
    return componentsMap.count(componentName);
}

Node* nodeFactory::createNodeWithType(const std::string& type) {
    if (nodes.count(type)) {
        return nodes[type]();
    }
    LOG_ERROR(CSTRING_FORMAT("Type of node '%s' not registered! Created default node.", type.c_str()));
    return Node::create();
}

void nodeFactory::init() {}

bool nodeFactory::registerCustomNodeType(const std::string& type, std::function<Node*()> node) {
    if (!inited) {
        CCLOGERROR("nodeFactory::registerCustomNodeType: No initialized instance of the class! Type %s not registered!", type.c_str());
        return false;
    }
    if (nodes.find(type) == nodes.end()) {
        nodes[type] = std::move(node);
        return true;
    }
    return false;
}
