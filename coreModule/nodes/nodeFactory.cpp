#include "nodeFactory.h"
#include "generic/coreModule/nodes/propertyTypes/forward.h"
#include "generic/coreModule/nodes/types/armatureNode.h"
#include "generic/coreModule/nodes/types/buttonNode.h"
#include "generic/coreModule/nodes/types/gridNode.h"
#include "generic/coreModule/nodes/types/node3d.h"
#include "generic/coreModule/nodes/types/soundButton.h"
#include "generic/debugModule/logManager.h"
#include "generic/utilityModule/stringUtility.h"
#include "spine/spine-cocos2dx.h"
#include "ui/CocosGUI.h"
#include <map>
#include <string>
#include <utility>

using namespace generic::coreModule;

std::map<std::string, propertyInterface*> nodeFactory::componentsMap = {};

nodeFactory* currentNodeFactory = nullptr;

nodeFactory::nodeFactory() {
    /// Default node types
    if (!inited) {
        inited = true;
        /// Core types
        nodes["node"] = []() { return cocos2d::Node::create(); };
        nodes["sprite"] = []() { return cocos2d::Sprite::create(); };
        nodes["sprite3d"] = []() { return cocos2d::Sprite3D::create(); };
        nodes["label"] = []() { return cocos2d::Label::create(); };
        nodes["buttonNode"] = []() { return buttonNode::create(); };
        nodes["layout"] = []() { return cocos2d::ui::Layout::create(); };
        nodes["layer"] = []() { return cocos2d::Layer::create(); };
        nodes["clippingNode"] = []() { return cocos2d::ClippingNode::create(); };
        nodes["scale9sprite"] = []() { return cocos2d::ui::Scale9Sprite::create(); };
        /// External types, in generic
        nodes["dragonbones"] = []() { return new armatureNode(); };
        nodes["spine"] = []() { return new spine::SkeletonAnimation(); };
        nodes["scrollView"] = []() { return cocos2d::ui::ScrollView::create(); };
        nodes["soundButton"] = []() { return soundButton::create(); };
        nodes["grid"] = []() { return gridNode::create(); };
        nodes["node3d"] = []() { return node3d::create(); };

        // prop types parser
        componentsMap["transformProperty"] = new transformProperty("transformProperty");
        componentsMap["labelProperty"] = new labelProperty("labelProperty");
        componentsMap["colorProperty"] = new colorProperty("colorProperty");
        componentsMap["gridProperty"] = new gridProperty("gridProperty");
        componentsMap["spineProperty"] = new spineProperty("spineProperty");
        componentsMap["spriteProperty"] = new spriteProperty("spriteProperty");
        componentsMap["dragonbonesProperty"] = new dragonbonesProperty("dragonbonesProperty");
        componentsMap["scrollViewProperty"] = new scrollViewProperty("scrollViewProperty");
        componentsMap["scale9SpriteProperty"] = new scale9SpriteProperty("scale9SpriteProperty");
        componentsMap["clipProperty"] = new clipProperty("clipProperty");
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
