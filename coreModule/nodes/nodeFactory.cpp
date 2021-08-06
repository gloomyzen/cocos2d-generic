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

// todo remove this later
static std::map<std::string, bool> usedArmature{};

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
    componentsMap.clear();
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
        if (object.HasMember("position")) {
            auto positions = object["position"].GetArray();
            if (positions.Size() == 2) {
                node->setPosition(positions[0].GetFloat(), positions[1].GetFloat());
            } else if (positions.Size() == 3) {
                node->setPosition3D(cocos2d::Vec3(positions[0].GetFloat(), positions[1].GetFloat(), positions[2].GetFloat()));
            } else {
                LOG_ERROR(CSTRING_FORMAT("Component '%s' has wrong '%s' position keys!",
                                              componentName.c_str(),
                                              std::to_string(positions.Size()).c_str()));
            }
        }
        if (object.HasMember("pivot")) {
            auto pivot = object["pivot"].GetArray();
            if (pivot.Size() == 2) {
                node->setPivotPoint(Vec2(pivot[0].GetFloat(), pivot[1].GetFloat()));
            } else {
                LOG_ERROR(CSTRING_FORMAT("Component '%s' has wrong '%s' pivot keys!",
                                              componentName.c_str(),
                                              std::to_string(pivot.Size()).c_str()));
            }
        }
        if (object.HasMember("anchor")) {
            auto anchor = object["anchor"].GetArray();
            if (anchor.Size() == 2) {
                node->setAnchorPoint(Vec2(anchor[0].GetFloat(), anchor[1].GetFloat()));
            } else {
                LOG_ERROR(CSTRING_FORMAT("Component '%s' has wrong '%s' anchor keys!",
                                              componentName.c_str(),
                                              std::to_string(anchor.Size()).c_str()));
            }
        }
        if (object.HasMember("size")) {
            auto size = object["size"].GetArray();
            if (size.Size() == 2) {
                bool onlyScaleMode = false;
                float scaleX, scaleY;
                scaleX = scaleY = .0f;
                if (auto sprite = dynamic_cast<cocos2d::Sprite*>(node)) {
                    if (sprite->getRenderMode() == cocos2d::Sprite::RenderMode::QUAD_BATCHNODE
                        || sprite->getRenderMode() == cocos2d::Sprite::RenderMode::POLYGON) {
                        onlyScaleMode = true;
                        const auto& content = sprite->getContentSize();
                        auto _size = cocos2d::Size();
                        _size.width = size[0].GetFloat();
                        _size.height = size[1].GetFloat();
                        if (content.width != 0) {
                            scaleX = _size.width / content.width;
                            scaleY = _size.height / content.height;
                        }
                    }
                }
                if (onlyScaleMode) {
                    node->setScaleX(scaleX);
                    node->setScaleY(scaleY);
                } else {
                    auto _size = cocos2d::Size();
                    _size.width = size[0].GetFloat();
                    _size.height = size[1].GetFloat();
                    node->setContentSize(_size);
                }
            } else {
                LOG_ERROR(CSTRING_FORMAT("Component '%s' has wrong '%s' size keys!",
                                              componentName.c_str(),
                                              std::to_string(size.Size()).c_str()));
            }
        }
        if (object.HasMember("scale") && object["scale"].IsNumber()) {
            auto scale = object["scale"].GetFloat();
            node->setScale(scale);
        } else if (object.HasMember("scale") && object["scale"].IsArray()) {
            auto scale = object["scale"].GetArray();
            if (scale.Size() == 2) {
                node->setScale(scale[0].GetFloat(), scale[1].GetFloat());
            } else {
                LOG_ERROR(CSTRING_FORMAT("Component '%s' has wrong '%s' scale keys!",
                                              componentName.c_str(),
                                              std::to_string(scale.Size()).c_str()));
            }
        }
        if (object.HasMember("rotation") && object["rotation"].IsNumber()) {
            node->setRotation(object["rotation"].GetFloat());
        }
        if (object.HasMember("rotation3d")) {
            auto rotation3d = object["rotation3d"].GetArray();
            if (rotation3d.Size() == 3) {
                node->setRotation3D(cocos2d::Vec3(rotation3d[0].GetFloat(), rotation3d[1].GetFloat(), rotation3d[2].GetFloat()));
            } else {
                LOG_ERROR(CSTRING_FORMAT("Component '%s' has wrong '%s' rotation3d keys!",
                                              componentName.c_str(),
                                              std::to_string(rotation3d.Size()).c_str()));
            }
        }
        if (object.HasMember("stretch")) {
            auto stretch = object["stretch"].GetArray();
            if (stretch.Size() == 2) {
                node->setStretch(stretch[0].GetFloat(), stretch[1].GetFloat());
            } else {
                LOG_ERROR(CSTRING_FORMAT("Component '%s' has wrong '%s' stretch keys!",
                                              componentName.c_str(),
                                              std::to_string(stretch.Size()).c_str()));
            }
        }
    } break;
    case eNodeFactory::SPRITE_COMPONENT: {
        if (auto button = dynamic_cast<buttonBase*>(node)) {
            if (object.HasMember("image") && object["image"].IsString()) {
                button->loadTexture(object["image"].GetString());
            }
        } else if (auto sprite3d = dynamic_cast<Sprite3D*>(node)) {
            if (object.HasMember("image") && object["image"].IsString()) {
                sprite3d->initWithFile(object["image"].GetString());
                sprite3d->setForce2DQueue(true);
                sprite3d->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
            }
        } else if (auto sprite = dynamic_cast<Sprite*>(node)) {
            std::string imagePath;
            bool isPoly = true;
            if (object.HasMember("image") && object["image"].IsString()) {
                imagePath = object["image"].GetString();
                if (imagePath.empty()) {
                    LOG_ERROR(CSTRING_FORMAT("Component '%s' has invalid image path!", componentName.c_str()));
                    break;
                }
                if (object.HasMember("polygon") && object["polygon"].IsBool()) {
                    isPoly = object["polygon"].GetBool();
                }

                if (isPoly) {
                    auto polygon = AutoPolygon::generatePolygon(imagePath);
                    sprite->initWithPolygon(polygon);
                } else {
                    sprite->initWithFile(imagePath);
                }
                if (object.HasMember("alphaMode") && object["alphaMode"].IsNumber()) {
                    switch (object["alphaMode"].GetInt()) {
                    case 1:
                        sprite->setBlendFunc(BlendFunc::ADDITIVE);
                    case 2:
                        sprite->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
                    case 3:
                        sprite->setBlendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED);
                    case 4:
                        sprite->setBlendFunc(BlendFunc::DISABLE);
                    default:
                        sprite->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
                        break;
                    }
                }
            } else {
                LOG_ERROR(CSTRING_FORMAT("Component '%s' no has image path!", componentName.c_str()));
                break;
            }
        } else {
            LOG_ERROR(CSTRING_FORMAT("Node '%s', component '%s' no has sprite node type!",
                                    node->getName().c_str(),
                                    componentName.c_str()));
        }
    } break;
    case eNodeFactory::LABEL_COMPONENT: {
        if (auto label = dynamic_cast<Label*>(node)) {
            if (object.HasMember("fontType") && object["fontType"].IsString() && object["fontType"].GetString() == std::string("ttf")) {
                TTFConfig font = label->getTTFConfig();
                if (font.fontFilePath.empty()) {
                    font.fontFilePath = "fonts/arial.ttf";
                }
                font.outlineSize = 0;

                if (object.HasMember("fontSize") && object["fontSize"].IsNumber()) {
                    font.fontSize = object["fontSize"].GetFloat() * 2;
                    label->setScale(0.5f);
                }
                if (object.HasMember("fontFile") && object["fontFile"].IsString()) {
                    font.fontFilePath = object["fontFile"].GetString();
                }
                if (object.HasMember("bold") && object["bold"].IsBool()) {
                    font.bold = object["bold"].GetBool();
                }
                if (object.HasMember("italic") && object["italic"].IsBool()) {
                    font.italics = object["italic"].GetBool();
                }
                if (object.HasMember("alight") && object["alight"].IsString()) {
                    TextHAlignment hAlignment = label->getTextAlignment();
                    auto alight = object["alight"].GetString();
                    if (alight == std::string("center")) {
                        hAlignment = TextHAlignment::CENTER;
                    } else if (alight == std::string("left")) {
                        hAlignment = TextHAlignment::LEFT;
                    } else if (alight == std::string("right")) {
                        hAlignment = TextHAlignment::RIGHT;
                    }
                    label->setAlignment(hAlignment);
                }
                if (object.HasMember("maxLineWidth") && object["maxLineWidth"].IsNumber()) {
                    label->setMaxLineWidth(object["maxLineWidth"].GetFloat());
                }
                label->setTTFConfig(font);
                if (object.HasMember("text") && object["text"].IsString()) {
                    label->setString(object["text"].GetString());
                }
            }
        } else {
            LOG_ERROR(CSTRING_FORMAT("Component '%s' no has label node type!", componentName.c_str()));
        }
    } break;
    case eNodeFactory::DRAGONBONES_COMPONENT: {
        if (auto dragonbones = dynamic_cast<armatureNode*>(node)) {
            if (object.HasMember("texFile") && object.HasMember("skeFile")) {
                if (usedArmature.find(object["name"].GetString()) == usedArmature.end()) {
                    // todo check file exists
                    CCFactory::getFactory()->loadTextureAtlasData(object["texFile"].GetString());
                    CCFactory::getFactory()->loadDragonBonesData(object["skeFile"].GetString());
                    usedArmature[object["name"].GetString()] = true;
                }
                auto bone = CCFactory::getFactory()->buildArmatureDisplay(object["name"].GetString());
                if (bone->getArmature()) {
                    if (object.HasMember("frameRate") && object["frameRate"].IsNumber()) {
                        bone->getArmature()->setCacheFrameRate(object["frameRate"].GetInt());
                    }
                    int playTimes = -1;
                    float fadeInTime = -1.f;
                    if (object.HasMember("playTimes") && object["playTimes"].IsNumber()) {
                        playTimes = object["playTimes"].GetInt();
                    }
                    if (object.HasMember("fadeInTime") && object["fadeInTime"].IsNumber()) {
                        fadeInTime = object["fadeInTime"].GetFloat();
                    }
                    if (object.HasMember("animation") && object["animation"].IsString()) {
                        if (bone->getAnimation()->hasAnimation(object["animation"].GetString())) {
                            bone->getAnimation()->fadeIn(object["animation"].GetString(), fadeInTime, playTimes);
                        } else {
                            LOG_ERROR(
                              CSTRING_FORMAT("Can't find animation '%s'", object["animation"].GetString()));
                        }
                    }
                    dragonbones->addChild(bone);
                } else {
                    LOG_ERROR("Can't get any armature from factory!");
                }
            }
        } else {
            LOG_ERROR(
              CSTRING_FORMAT("Component '%s' no has DragonBones node type!", componentName.c_str()));
        }
    } break;
    case eNodeFactory::SPINE_COMPONENT: {
        if (auto spine = dynamic_cast<spine::SkeletonAnimation*>(node)) {
            auto scale = 1.f;
            if (object.HasMember("scale") && object["scale"].IsNumber()) {
                scale = object["scale"].GetFloat();
            }
            if (object.HasMember("file") && object["file"].IsString()) {
                auto atlas = STRING_FORMAT("%s.atlas", object["file"].GetString());
                auto skel = STRING_FORMAT("%s.skel", object["file"].GetString());
                auto json = STRING_FORMAT("%s.json", object["file"].GetString());
                if (cocos2d::FileUtils::getInstance()->isFileExist(atlas) && cocos2d::FileUtils::getInstance()->isFileExist(skel)) {
                    spine->initWithBinaryFile(skel, atlas, scale);
                    spine->autorelease();
                } else if (cocos2d::FileUtils::getInstance()->isFileExist(atlas) && cocos2d::FileUtils::getInstance()->isFileExist(json)) {
                    spine->initWithJsonFile(json, atlas, scale);
                    spine->autorelease();
                } else {
                    LOG_ERROR(CSTRING_FORMAT("Can't get atlas or binary file for spine '%s'!",
                                                  node->getName().c_str()));
                }
            }
            auto loop = false;
            if (object.HasMember("loop") && object["loop"].IsBool()) {
                loop = object["loop"].GetBool();
            }
            if (object.HasMember("animation") && object["animation"].IsString()) {
                auto animation = object["animation"].GetString();
                spine->setAnimation(1, animation, loop);
            }
            if (object.HasMember("skin") && object["skin"].IsString()) {
                spine->setSkin(object["skin"].GetString());
            }
        } else {
            LOG_ERROR(
              CSTRING_FORMAT("Component '%s' no has DragonBones node type!", componentName.c_str()));
        }
    } break;
    case eNodeFactory::COLOR_COMPONENT: {
        if (object.HasMember("color") && object["color"].IsArray()) {
            auto color = object["color"].GetArray();
            if (color.Size() >= 3 && color.Size() <= 4) {
                Color3B rgb;
                rgb.r = static_cast<uint8_t>(color[0].GetFloat());
                rgb.g = static_cast<uint8_t>(color[1].GetFloat());
                rgb.b = static_cast<uint8_t>(color[2].GetFloat());
                node->setColor(rgb);
                if (color.Size() == 4) {
                    node->setOpacity(static_cast<uint8_t>(color[3].GetFloat()));
                }
            } else {
                LOG_ERROR(CSTRING_FORMAT("Component '%s' has wrong '%s' color keys!",
                                              componentName.c_str(),
                                              std::to_string(color.Size()).c_str()));
            }
        }
    } break;
    case eNodeFactory::SCROLL_VIEW_COMPONENT: {
        if (auto scrollNode = dynamic_cast<ui::ScrollView*>(node)) {
            // direction
            auto direction = ui::ScrollView::Direction::NONE;
            if (object.HasMember("direction") && object["direction"].IsString()) {
                std::string conf = object["direction"].GetString();
                if (conf == "none") {
                    direction = ui::ScrollView::Direction::NONE;
                } else if (conf == "horizontal") {
                    direction = ui::ScrollView::Direction::HORIZONTAL;
                } else if (conf == "vertical") {
                    direction = ui::ScrollView::Direction::VERTICAL;
                } else if (conf == "both") {
                    direction = ui::ScrollView::Direction::BOTH;
                }
            }
            scrollNode->setDirection(direction);
            // bounce
            bool bounce = false;
            if (object.HasMember("bounce") && object["bounce"].IsBool()) {
                bounce = object["bounce"].GetBool();
            }
            scrollNode->setBounceEnabled(bounce);
            if (object.HasMember("bounceOffset") && object["bounceOffset"].IsArray()) {
                auto tempArray = object["bounceOffset"].GetArray();
                if (tempArray[0].IsNumber() && tempArray[1].IsNumber()) {
                    scrollNode->setBounceOffset(tempArray[0].GetFloat(), tempArray[1].GetFloat());
                }
            }
            // scroll bar
            bool scrollBar = false;
            if (object.HasMember("scrollBar") && object["scrollBar"].IsBool()) {
                scrollBar = object["scrollBar"].GetBool();
            }
            scrollNode->setScrollBarEnabled(scrollBar);
            // inertial scroll (best performance with horizontal or vertical)
            bool inertialScroll = false;
            if (object.HasMember("inertialScroll") && object["inertialScroll"].IsBool()) {
                inertialScroll = object["inertialScroll"].GetBool();
            }
            scrollNode->setInertiaScrollEnabled(inertialScroll);
            // inner container size
            if (object.HasMember("containerSize") && object["containerSize"].IsArray()) {
                auto containerSize = object["containerSize"].GetArray();
                if (containerSize.Size() == 2) {
                    cocos2d::Size innerSize;
                    innerSize.width = containerSize[0].GetFloat();
                    innerSize.height = containerSize[1].GetFloat();
                    scrollNode->setInnerContainerSize(innerSize);
                }
            }
            // start scroll position
            if (object.HasMember("scrollPosPercent") && object["scrollPosPercent"].IsArray()) {
                auto scrollPosPercent = object["scrollPosPercent"].GetArray();
                if (scrollPosPercent.Size() == 2) {
                    cocos2d::Vec2 jumpPos;
                    jumpPos.x = scrollPosPercent[0].GetFloat();
                    jumpPos.y = scrollPosPercent[1].GetFloat();
                    if (direction == ui::ScrollView::Direction::HORIZONTAL) {
                        scrollNode->jumpToPercentHorizontal(jumpPos.x);
                    } else if (direction == ui::ScrollView::Direction::VERTICAL) {
                        scrollNode->jumpToPercentVertical(jumpPos.y);
                    } else if (direction == ui::ScrollView::Direction::BOTH) {
                        scrollNode->jumpToPercentBothDirection(jumpPos);
                    }
                }
            }
        }
    }
    case eNodeFactory::GRID_COMPONENT: {
        if (auto grid = dynamic_cast<gridNode*>(node)) {
            if (object.HasMember("marginX") && object["marginX"].IsArray()) {
                auto array = object["marginX"].GetArray();
                if (array.Size() == 2 && array[0].IsInt() && array[1].IsInt()) {
                    grid->setMarginX(array[0].GetInt(), array[1].GetInt());
                }
            }
            if (object.HasMember("marginY") && object["marginY"].IsArray()) {
                auto array = object["marginY"].GetArray();
                if (array.Size() == 2 && array[0].IsInt() && array[1].IsInt()) {
                    grid->setMarginY(array[0].GetInt(), array[1].GetInt());
                }
            }
            if (object.HasMember("paddingX") && object["paddingX"].IsArray()) {
                auto array = object["paddingX"].GetArray();
                if (array.Size() == 2 && array[0].IsInt() && array[1].IsInt()) {
                    grid->setPaddingX(array[0].GetInt(), array[1].GetInt());
                }
            }
            if (object.HasMember("paddingY") && object["paddingY"].IsArray()) {
                auto array = object["paddingY"].GetArray();
                if (array.Size() == 2 && array[0].IsInt() && array[1].IsInt()) {
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
                if (size.Size() == 2) {
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
                if (color.Size() == 4) {
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
