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
        if (auto grid = dynamic_cast<gridNode*>(node)) {
            if (object.HasMember("marginX") && object["marginX"].IsArray()) {
                auto array = object["marginX"].GetArray();
                if (array.Size() == 2u && array[0].IsInt() && array[1].IsInt()) {
                    grid->setMarginX(array[0].GetInt(), array[1].GetInt());
                }
            }
            if (object.HasMember("marginY") && object["marginY"].IsArray()) {
                auto array = object["marginY"].GetArray();
                if (array.Size() == 2u && array[0].IsInt() && array[1].IsInt()) {
                    grid->setMarginY(array[0].GetInt(), array[1].GetInt());
                }
            }
            if (object.HasMember("paddingX") && object["paddingX"].IsArray()) {
                auto array = object["paddingX"].GetArray();
                if (array.Size() == 2u && array[0].IsInt() && array[1].IsInt()) {
                    grid->setPaddingX(array[0].GetInt(), array[1].GetInt());
                }
            }
            if (object.HasMember("paddingY") && object["paddingY"].IsArray()) {
                auto array = object["paddingY"].GetArray();
                if (array.Size() == 2u && array[0].IsInt() && array[1].IsInt()) {
                    grid->setPaddingY(array[0].GetInt(), array[1].GetInt());
                }
            }
            if (object.HasMember("col") && object["col"].IsInt()) {
                grid->setColumns(object["col"].GetInt());
            }
            if (object.HasMember("row") && object["row"].IsInt()) {
                grid->setRows(object["row"].GetInt());
            }
            if (object.HasMember("direction") && object["direction"].IsString()) {
                auto direction = gridNode::getGridDirectionByString(object["direction"].GetString());
                grid->setDirection(direction);
            }
            //				if (object.HasMember("alignX") && object["alignX"].IsString()) {
            //					auto align = gridNode::getGridAlignXByString(object["alignX"].GetString());
            //					grid->setAlignX(align);
            //				}
            //				if (object.HasMember("alignY") && object["alignY"].IsString()) {
            //					auto align = gridNode::getGridAlignYByString(object["alignY"].GetString());
            //					grid->setAlignY(align);
            //				}
        }
    } break;
    case eNodeFactory::SCALE9SPRITE_COMPONENT: {
        if (auto sprite = dynamic_cast<ui::Scale9Sprite*>(node)) {
            cocos2d::Rect sliceRect = cocos2d::Rect::ZERO;
            if (object.HasMember("image") && object["image"].IsString()) {
                std::string imagePath;
                imagePath = object["image"].GetString();
                if (imagePath.empty()) {
                    LOG_ERROR(CSTRING_FORMAT("Component '%s' has invalid image path!", componentName.c_str()));
                    break;
                }
                sprite->initWithFile(imagePath);
            }
            if (object.HasMember("slice9") && object["slice9"].IsObject()) {
                auto slice9 = object["slice9"].GetObject();
                if (slice9.HasMember("x") && slice9["x"].IsNumber()) {
                    sliceRect.origin.x = slice9["x"].GetFloat();
                }
                if (slice9.HasMember("y") && slice9["y"].IsNumber()) {
                    sliceRect.origin.y = slice9["y"].GetFloat();
                }
                if (slice9.HasMember("width") && slice9["width"].IsNumber()) {
                    sliceRect.size.width = slice9["width"].GetFloat();
                }
                if (slice9.HasMember("height") && slice9["height"].IsNumber()) {
                    sliceRect.size.height = slice9["height"].GetFloat();
                }
                sprite->setCapInsets(sliceRect);
            }
            if (object.HasMember("size") && object["size"].IsArray()) {
                auto _size = cocos2d::Size();
                auto size = object["size"].GetArray();
                if (size.Size() == 2u) {
                    _size.width = size[0].GetFloat();
                    _size.height = size[1].GetFloat();
                    sprite->setContentSize(_size);
                }
            }
        }
    } break;
    case eNodeFactory::CLIP_COMPONENT: {
        if (auto clipNode = dynamic_cast<ClippingNode*>(node)) {
            bool inverted = false;
            bool autoUpdate = false;
            auto stencilColor = Color4F::BLACK;
            if (object.HasMember("inverted") && object["inverted"].IsBool()) {
                inverted = object["inverted"].GetBool();
            }
            if (object.HasMember("autoUpdate") && object["autoUpdate"].IsBool()) {
                autoUpdate = object["autoUpdate"].GetBool();
            }
            if (object.HasMember("color") && object["color"].IsArray()) {
                auto color = object["color"].GetArray();
                if (color.Size() == 4u) {
                    Color4F rgba;
                    rgba.r = color[0].GetFloat() / 255;
                    rgba.g = color[1].GetFloat() / 255;
                    rgba.b = color[2].GetFloat() / 255;
                    rgba.a = color[3].GetFloat() / 255;
                    stencilColor = rgba;
                }
            }
//            if (object.HasMember("nodeName") && object["nodeName"].IsString()) {
//                auto stencil = DrawNode::create();
//                auto name = object["nodeName"].GetString();
//                stencil->setName(name);
//                if (allProperties.HasMember(name) && allProperties[name].IsObject()) {
//                    auto nodeObj = allProperties[name].GetObject();
//                    if (nodeObj.HasMember("transformComponent") && nodeObj["transformComponent"].IsObject()) {
//                        auto componentObj = nodeObj["transformComponent"].GetObject();
//                        readComponent(stencil, "transformComponent", componentObj, allProperties);
//                    }
//                }
//                clipNode->setInverted(inverted);
//                clipNode->addChild(stencil);
//                stencil->drawSolidRect(Vec2::ZERO, stencil->getContentSize(), stencilColor);
//                clipNode->setStencil(stencil);
//            } else {
//                LOG_ERROR(STRING_FORMAT("Component '%s' has invalid nodeName!", componentName.c_str()));
//            }
            auto stencil = drawNodeBase::create();
            clipNode->setStencil(stencil);
            clipNode->addChild(stencil);
            stencil->setDrawColor(stencilColor);
            stencil->setAutoUpdateEnabled(autoUpdate);
            stencil->forceUpdateRect();
            clipNode->setInverted(inverted);
        }
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
