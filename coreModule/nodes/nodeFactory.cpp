#include "nodeFactory.h"
#include <map>
#include <string>
#include <utility>
#include "common/debugModule/logManager.h"
#include "ui/CocosGUI.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"
#include "common/coreModule/nodes/armatureHolderNode.h"
#include "common/coreModule/nodes/spriteParameters.h"

using namespace common::coreModule;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace dragonBones;

std::map<std::string, eNodeFactory> componentsMap = {
		{"transformComponent",  eNodeFactory::TRANSFORM_COMPONENT},
		{"spriteComponent",     eNodeFactory::SPRITE_COMPONENT},
		{"animspriteComponent", eNodeFactory::ANIMSPRITE_COMPONENT},
		{"labelComponent", eNodeFactory::LABEL_COMPONENT},
		{"buttonComponent", eNodeFactory::BUTTON_COMPONENT},
		{"dragonbonesComponent", eNodeFactory::DRAGONBONES_COMPONENT},
		{"colorComponent", eNodeFactory::COLOR_COMPONENT},
		{"scrollViewComponent", eNodeFactory::SCROLL_VIEW_COMPONENT},
};
std::map<std::string, std::function<Node*()>> nodes{};

//todo remove this later
static std::map<std::string, bool> usedArmature{};

nodeFactory *currentNodeFactory = nullptr;

nodeFactory::nodeFactory() {
/// Default node types
	if (!inited) {
		inited = true;
		///Core types
		nodes["node"] = []()->Node* { return new Node(); };
		nodes["sprite"] = []()->Sprite* { return new Sprite(); };
		nodes["label"] = []()->Label* { return new Label(); };
		nodes["button"] = []()->Button* { return new Button(); };
		nodes["layout"] = []()->Layout* { return new Layout(); };
		nodes["clippingNode"] = []()->ClippingNode* {
			//todo need fix this
			ClippingNode * clipper = ClippingNode::create();
			DrawNode * stencil = DrawNode::create();
			stencil->setName("clipperStencil");
			stencil->drawSolidCircle(Vec2(100,
										  100), 100, 0, 200, Color4F::MAGENTA);
			clipper->setStencil(stencil);
			clipper->setInverted(true);
			return clipper;
		};
		///External types
		nodes["dragonbones"] = []() { return new armatureHolderNode(); };
		nodes["scrollView"] = []() { return ScrollView::create(); };
	}
}

nodeFactory::~nodeFactory() = default;

nodeFactory &nodeFactory::getInstance() {
	if (currentNodeFactory == nullptr) {
		currentNodeFactory = new nodeFactory();
	}
	return *currentNodeFactory;
}

void nodeFactory::getComponents(Node *node, const std::string &componentName, const rapidjson::GenericValue<rapidjson::UTF8<char>>::Object &object) {
	if (node == nullptr)
		return;

	if (!hasRegisteredComponent(componentName)) {
		LOG_ERROR("nodeFactory::getComponents: Component '" + componentName + "' was not found!");
		return;
	}

	eNodeFactory needle = componentsMap[componentName];

	switch (needle) {
		case TRANSFORM_COMPONENT: {
			if (object.HasMember("position")) {
				auto positions = object["position"].GetArray();
				if (positions.Size() == 2) {
					node->setPosition(positions[0].GetFloat(), positions[1].GetFloat());
				} else {
					LOG_ERROR(StringUtils::format("nodeFactory::getComponents: Component '%s' has wrong '%s' position keys!",
												  componentName.c_str(), std::to_string(positions.Size()).c_str()));
				}
			}
			if (object.HasMember("pivot")) {
				auto pivot = object["pivot"].GetArray();
				if (pivot.Size() == 2) {
					node->setPivotPoint(Vec2(pivot[0].GetFloat(), pivot[1].GetFloat()));
				} else {
					LOG_ERROR(StringUtils::format("nodeFactory::getComponents: Component '%s' has wrong '%s' pivot keys!",
												  componentName.c_str(), std::to_string(pivot.Size()).c_str()));
				}
			}
			if (object.HasMember("anchor")) {
				auto anchor = object["anchor"].GetArray();
				if (anchor.Size() == 2) {
					node->setAnchorPoint(Vec2(anchor[0].GetFloat(), anchor[1].GetFloat()));
				} else {
					LOG_ERROR(StringUtils::format("nodeFactory::getComponents: Component '%s' has wrong '%s' anchor keys!",
												  componentName.c_str(), std::to_string(anchor.Size()).c_str()));
				}
			}
			if (object.HasMember("size")) {
				auto size = object["size"].GetArray();
				if (size.Size() == 2) {
					auto _size = cocos2d::Size();
					_size.width = size[0].GetFloat();
					_size.height = size[1].GetFloat();
					node->setContentSize(_size);
				} else {
					LOG_ERROR(StringUtils::format("nodeFactory::getComponents: Component '%s' has wrong '%s' size keys!",
												  componentName.c_str(), std::to_string(size.Size()).c_str()));
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
					LOG_ERROR(StringUtils::format("nodeFactory::getComponents: Component '%s' has wrong '%s' scale keys!",
												  componentName.c_str(), std::to_string(scale.Size()).c_str()));
				}
			}
			if (object.HasMember("rotation") && object["rotation"].IsNumber()) {
				node->setRotation(object["rotation"].GetFloat());
			}
			if (object.HasMember("stretch")) {
				auto stretch = object["stretch"].GetArray();
				if (stretch.Size() == 2) {
					auto visibleSize = Director::getInstance()->getVisibleSize();
					auto _size = cocos2d::Size();
					_size.width = visibleSize.width * stretch[0].GetFloat();
					_size.height = visibleSize.height * stretch[1].GetFloat();
					node->setContentSize(_size);
				} else {
					LOG_ERROR(StringUtils::format("nodeFactory::getComponents: Component '%s' has wrong '%s' stretch keys!",
												  componentName.c_str(), std::to_string(stretch.Size()).c_str()));
				}
			}
		}
			break;
		case SPRITE_COMPONENT: {
			if (auto sprite = dynamic_cast<Sprite*>(node)) {
				if (object.HasMember("image") && object["image"].IsString()) {
					sprite->initWithFile(object["image"].GetString());
				}
				if (object.HasMember("pixel") && object["pixel"].IsBool() && object["pixel"].GetBool()) {
					spriteParameters::setCorrectPixelartTexture(sprite);
				}
				//spriteParameters
			} else {
				LOG_ERROR(StringUtils::format("nodeFactory::getComponents: Component '%s' no has sprite node type!", componentName.c_str()));
			}
		}
			break;
		case ANIMSPRITE_COMPONENT:
			break;
		case LABEL_COMPONENT: {
			if (auto label = dynamic_cast<Label*>(node)) {
				if (object.HasMember("fontType") && object["fontType"].IsString() && object["fontType"].GetString() == std::string("ttf")) {
					TTFConfig font = label->getTTFConfig();
					if (font.fontFilePath.empty()) {
						font.fontFilePath = "fonts/arial.ttf";
					}

					TextHAlignment hAlignment = label->getTextAlignment();
					float maxLineWidth = label->getMaxLineWidth();
					if (object.HasMember("fontSize") && object["fontSize"].IsNumber()) {
						font.fontSize = object["fontSize"].GetFloat();
					}
					if (object.HasMember("fontFile") && object["fontFile"].IsString()) {
						font.fontFilePath = object["fontFile"].GetString();
					}
					if (object.HasMember("bold") && object["bold"].IsBool()) {
						font.bold = object["bold"].GetBool();
					}
					if (object.HasMember("alight") && object["alight"].IsString()) {
						auto alight = object["alight"].GetString();
						if (alight == std::string("center")) {
							hAlignment = TextHAlignment::CENTER;
						} else
						if (alight == std::string("left")) {
							hAlignment = TextHAlignment::LEFT;
						} else
						if (alight == std::string("right")) {
							hAlignment = TextHAlignment::RIGHT;
						}
					}
					if (object.HasMember("maxLineWidth") && object["maxLineWidth"].IsNumber()) {
						maxLineWidth = object["maxLineWidth"].GetFloat();
					}
					if (object.HasMember("text") && object["text"].IsString()) {
						label->initWithTTF(font, object["text"].GetString(), hAlignment, static_cast<int>(maxLineWidth));
					} else {
						label->setTTFConfig(font);
						label->setAlignment(hAlignment);
						label->setMaxLineWidth(maxLineWidth);
					}
				}
			} else {
				LOG_ERROR(StringUtils::format("nodeFactory::getComponents: Component '%s' no has label node type!", componentName.c_str()));
			}
		}
			break;
		case BUTTON_COMPONENT: {
			if (auto button = dynamic_cast<Button*>(node)) {
				std::string normalImg{};
				std::string selectedImg{};
				std::string disabledImg{};
				if (object.HasMember("normalImage") && object["normalImage"].IsString()) {
					normalImg = object["normalImage"].GetString();
				}
				if (object.HasMember("selectedImage") && object["selectedImage"].IsString()) {
					selectedImg = object["selectedImage"].GetString();
				}
				if (object.HasMember("disabledImage") && object["disabledImage"].IsString()) {
					disabledImg = object["disabledImage"].GetString();
				}
				if (!normalImg.empty()) {
					button->init(normalImg, !selectedImg.empty() ? selectedImg : "", !disabledImg.empty() ? disabledImg : "");
				}
			} else {
				LOG_ERROR(StringUtils::format("nodeFactory::getComponents: Component '%s' no has button node type!", componentName.c_str()));
			}
		}
			break;
		case DRAGONBONES_COMPONENT: {
			if (auto dragonbones = dynamic_cast<armatureHolderNode*>(node)) {
				if (object.HasMember("texFile") && object.HasMember("skeFile")) {
					if (usedArmature.find(object["name"].GetString()) == usedArmature.end()) {
						auto test = CCFactory::getFactory()->loadDragonBonesData(object["skeFile"].GetString());
						auto textData = CCFactory::getFactory()->loadTextureAtlasData(object["texFile"].GetString());
						usedArmature[textData->name] = true;
					}
					auto bone = CCFactory::getFactory()->buildArmatureDisplay(object["name"].GetString());
					if (bone->getArmature()) {
						//todo get attr: frameRate, animationName, skinName, isLoop
						bone->getArmature()->setCacheFrameRate(24);
						bone->getAnimation()->play("Idle", 9999);
						dragonbones->addArmature(bone);
					} else {
						LOG_ERROR(StringUtils::format("nodeFactory::getComponents: Can't get any armature from factory!"));
					}

				} else if (object.HasMember("file")) {
					//todo get from file
				}
			} else {
				LOG_ERROR(StringUtils::format("nodeFactory::getComponents: Component '%s' no has DragonBones node type!", componentName.c_str()));
			}
		}
			break;
		case COLOR_COMPONENT: {
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
					LOG_ERROR(StringUtils::format("nodeFactory::getComponents: Component '%s' has wrong '%s' color keys!",
												  componentName.c_str(), std::to_string(color.Size()).c_str()));
				}
			}
		}
			break;
		case SCROLL_VIEW_COMPONENT: {
			if (auto scrollNode = dynamic_cast<ui::ScrollView*>(node)) {
				//direction
				auto direction = ui::ScrollView::Direction::NONE;
				if (object.HasMember("direction") && object["direction"].IsString()) {
					std::string conf = object["direction"].GetString();
					if (conf == "none") {
						direction = ui::ScrollView::Direction::NONE;
					} else
					if (conf == "horizontal") {
						direction = ui::ScrollView::Direction::HORIZONTAL;
					} else
					if (conf == "vertical") {
						direction = ui::ScrollView::Direction::VERTICAL;
					} else
					if (conf == "both") {
						direction = ui::ScrollView::Direction::BOTH;
					}
				}
				scrollNode->setDirection(direction);
				//bounce
				bool bounce = false;
				if (object.HasMember("bounce") && object["bounce"].IsBool()) {
					bounce = object["bounce"].GetBool();
				}
				scrollNode->setBounceEnabled(bounce);
				//scroll bar
				bool scrollBar = false;
				if (object.HasMember("scrollBar") && object["scrollBar"].IsBool()) {
					scrollBar = object["scrollBar"].GetBool();
				}
				scrollNode->setScrollBarEnabled(scrollBar);
				//inertial scroll (best performance with horizontal or vertical)
				bool inertialScroll = false;
				if (object.HasMember("inertialScroll") && object["inertialScroll"].IsBool()) {
					inertialScroll = object["inertialScroll"].GetBool();
				}
				scrollNode->setInertiaScrollEnabled(inertialScroll);
				//inner container size
				if (object.HasMember("containerSize") && object["containerSize"].IsArray()) {
					auto containerSize = object["containerSize"].GetArray();
					if (containerSize.Size() == 2) {
						cocos2d::Size innerSize;
						innerSize.width = containerSize[0].GetFloat();
						innerSize.height = containerSize[1].GetFloat();
						scrollNode->setInnerContainerSize( innerSize );
					}
				}
				//start scroll position
				//tip: will work only with both scroll direction
				if (object.HasMember("scrollPosPercent") && object["scrollPosPercent"].IsArray()) {
					auto scrollPosPercent = object["scrollPosPercent"].GetArray();
					if (scrollPosPercent.Size() == 2) {
						cocos2d::Vec2 jumpPos;
						jumpPos.x = scrollPosPercent[0].GetFloat();
						jumpPos.y = scrollPosPercent[1].GetFloat();
						if (direction == ui::ScrollView::Direction::HORIZONTAL) {
							scrollNode->jumpToPercentHorizontal(jumpPos.x);
						} else
						if (direction == ui::ScrollView::Direction::VERTICAL) {
							scrollNode->jumpToPercentVertical(jumpPos.y);
						} else
						if (direction == ui::ScrollView::Direction::BOTH) {
							scrollNode->jumpToPercentBothDirection( jumpPos );
						}

					}
				}
			}
		}
			break;
	}
}

bool nodeFactory::hasRegisteredComponent(const std::string &componentName) {
	return (componentsMap.find(componentName) != componentsMap.end());
}

Node *nodeFactory::createNodeWithType(const std::string &type) {
	if (nodes.count(type)) {
		return nodes[type]();
	}
	LOG_ERROR("nodeFactory::createNodeWithType: Type of node '" + type + "' not registered! Created default node.");
	return Node::create();
}

void nodeFactory::init() {

}

bool nodeFactory::registerCustomNodeType(const std::string& type, std::function<Node *()> node) {
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
