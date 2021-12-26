#include "nodeFactory.h"
#include "generic/coreModule/nodes/propertyTypes/headers.h"
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
#include <utility>

using namespace generic::coreModule;

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
        nodes["aseprite"] = []() { return new asepriteNode(); };

        // prop types parser
        propertiesMap["transformProperty"] = new transformProperty("transformProperty");
        propertiesMap["labelProperty"] = new labelProperty("labelProperty");
        propertiesMap["colorProperty"] = new colorProperty("colorProperty");
        propertiesMap["gridProperty"] = new gridProperty("gridProperty");
        propertiesMap["spineProperty"] = new spineProperty("spineProperty");
        propertiesMap["spriteProperty"] = new spriteProperty("spriteProperty");
        propertiesMap["dragonbonesProperty"] = new dragonbonesProperty("dragonbonesProperty");
        propertiesMap["scrollViewProperty"] = new scrollViewProperty("scrollViewProperty");
        propertiesMap["scale9SpriteProperty"] = new scale9SpriteProperty("scale9SpriteProperty");
        propertiesMap["clipProperty"] = new clipProperty("clipProperty");
        propertiesMap["asepriteProperty"] = new asepriteProperty("asepriteProperty");

        propertyPriorityList = { "spriteProperty", "labelProperty", "dragonbonesProperty", "spineProperty", "scale9SpriteProperty", "asepriteProperty",
                                 "transformProperty", "colorProperty", "scrollViewProperty", "gridProperty", "clipProperty" };
    }
}

nodeFactory::~nodeFactory() = default;

void nodeFactory::cleanup() {
    nodes.clear();
    for (auto &item : propertiesMap) {
        delete item.second;
        item.second = nullptr;
    }
    propertiesMap.clear();
    delete currentNodeFactory;
    currentNodeFactory = nullptr;
}

nodeFactory& nodeFactory::getInstance() {
    if (currentNodeFactory == nullptr) {
        currentNodeFactory = new nodeFactory();
    }
    return *currentNodeFactory;
}

void nodeFactory::readProperty(cocos2d::Node* node,
                                const std::string& propertyName,
                                const jsonObject& object) {
    if (node == nullptr)
        return;

    if (!hasRegisteredProperty(propertyName)) {
        LOG_ERROR(CSTRING_FORMAT("Property '%s' was not registered!", propertyName.c_str()));
        return;
    }
    node->setCascadeOpacityEnabled(true);
    node->setCascadeColorEnabled(true);
    if (propertiesMap.count(propertyName) && propertiesMap[propertyName]) {
        propertiesMap[propertyName]->parseProperty(node, object);
    }
}

bool nodeFactory::hasRegisteredProperty(const std::string& propertyName) {
    return propertiesMap.count(propertyName);
}

cocos2d::Node* nodeFactory::createNodeWithType(const std::string& type) {
    if (nodes.count(type)) {
        return nodes[type]();
    }
    LOG_ERROR(CSTRING_FORMAT("Type of node '%s' not registered! Created default node.", type.c_str()));
    return cocos2d::Node::create();
}

void nodeFactory::init() {}

bool nodeFactory::registerCustomNodeType(const std::string& type, std::function<cocos2d::Node*()> node) {
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
