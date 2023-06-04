#include "nodeFactory.h"
#include "generic/coreModule/nodes/propertyTypes/headers.h"
#include "generic/coreModule/nodes/types/armatureNode.h"
#include "generic/coreModule/nodes/types/gridNode.h"
#include "generic/coreModule/nodes/types/node3d.h"
#include "generic/coreModule/nodes/types/soundButton.h"
#include "generic/coreModule/nodes/types/asepriteNode.h"
#include "generic/utilityModule/logManager.h"
#include "generic/utilityModule/stringUtility.h"
#include "generic/coreModule/components/transformComponent.h"
#include "spine/spine-cocos2dx.h"
#include "ui/CocosGUI.h"
#include <map>
#include <utility>

using namespace generic::coreModule;

nodeFactory* nodeFactory::pInstance = nullptr;
bool nodeFactory::destroyed = false;

nodeFactory::nodeFactory() {
    /// Default node types
    /// Core types
    nodes["node"] = []() { return ax::Node::create(); };
    nodes["sprite"] = []() { return ax::Sprite::create(); };
    nodes["sprite3d"] = []() { return ax::Sprite3D::create(); };
    nodes["label"] = []() { return ax::Label::create(); };
    nodes["layout"] = []() { return ax::ui::Layout::create(); };
    nodes["layer"] = []() { return ax::Layer::create(); };
    nodes["clippingNode"] = []() { return ax::ClippingNode::create(); };
    nodes["scale9Sprite"] = []() { return ax::ui::Scale9Sprite::create(); };
    /// External types, in generic
    nodes["dragonbones"] = []() { return armatureNode::create(); };
    nodes["spine"] = []() { return new spine::SkeletonAnimation(); };
    nodes["scrollView"] = []() { return ax::ui::ScrollView::create(); };
    nodes["soundButton"] = []() { return soundButton::create(); };
    nodes["grid"] = []() { return gridNode::create(); };
    nodes["node3d"] = []() { return node3d::create(); };
    nodes["aseprite"] = []() { return asepriteNode::create(); };

    // prop types parser
    propertiesMap["baseProperty"] = new baseProperty("baseProperty");
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
                             "colorProperty", "scrollViewProperty", "gridProperty", "clipProperty", "baseProperty" };
}

nodeFactory::~nodeFactory() {
    nodes.clear();
    for (auto &item : propertiesMap) {
        delete item.second;
        item.second = nullptr;
    }
    propertiesMap.clear();
};

void nodeFactory::cleanup() {
    destroyed = true;
    pInstance = nullptr;
}

nodeFactory& nodeFactory::getInstance() {
    if (!pInstance) {
        if (destroyed) {
            onDeadReference();
        } else {
            create();
        }
    }
    return *pInstance;
}

void nodeFactory::create() {
    static nodeFactory instance;
    pInstance = &instance;
}

void nodeFactory::onDeadReference() {
    AXASSERT(false, "Founded dead reference!");
}

void nodeFactory::readProperty(ax::Node* node,
                                const std::string& propertyName,
                                const jsonObject& object) {
    if (node == nullptr)
        return;

    if (!hasRegisteredProperty(propertyName)) {
        LOG_ERROR("Property '{}' was not registered", propertyName.c_str());
        return;
    }
    if (propertiesMap.count(propertyName) && propertiesMap[propertyName]) {
        propertiesMap[propertyName]->parseProperty(node, object);
    }
}

bool nodeFactory::hasRegisteredProperty(const std::string& propertyName) {
    return propertiesMap.count(propertyName);
}

ax::Node* nodeFactory::createNodeWithType(const std::string& type) {
    ax::Node* node;
    if (nodes.count(type) != 0u) {
        node = nodes[type]();
    } else {
        LOG_ERROR("Type of node '{}' not registered. Created default node.", type.c_str());
        node = ax::Node::create();
    }
    auto component = dynamic_cast<transformComponent*>(node->getComponent(transformComponent::TRANSFORM_COMPONENT_NAME));
    if (!component) {
        component = new transformComponent();
        node->addComponent(component);
    }
    return node;
}

bool nodeFactory::registerCustomNodeType(const std::string& type, std::function<ax::Node*()> node) {
    if (nodes.count(type) == 0u) {
        nodes[type] = std::move(node);
        return true;
    }
    return false;
}
