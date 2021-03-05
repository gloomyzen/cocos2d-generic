#include "gridNode.h"

using namespace common::coreModule;

static std::map<std::string, gridNode::eGridDirection> gridDirectionMap = {
    { "horizontal", gridNode::eGridDirection::HORIZONTAL },
    { "vertical", gridNode::eGridDirection::VERTICAL },
};
static std::map<std::string, gridNode::eGridAlignX> gridAlignXMap = {
    { "top", gridNode::eGridAlignX::TOP },
    { "bottom", gridNode::eGridAlignX::BOTTOM },
};

static std::map<std::string, gridNode::eGridAlignY> gridAlignYMap = {
    { "left", gridNode::eGridAlignY::LEFT },
    { "right", gridNode::eGridAlignY::RIGHT },
};

gridNode::gridNode() {
    marginX = { 0, 0 };
    marginY = { 0, 0 };
    paddingX = { 0, 0 };
    paddingY = { 0, 0 };
}

gridNode::~gridNode() {}

gridNode::eGridDirection gridNode::getGridDirectionByString(const std::string& type) {
    if (gridDirectionMap.find(type) != gridDirectionMap.end()) {
        return gridDirectionMap[type];
    }
    return gridNode::eGridDirection::VERTICAL;
}

gridNode::eGridAlignX gridNode::getGridAlignXByString(const std::string& type) {
    if (gridAlignXMap.find(type) != gridAlignXMap.end()) {
        return gridAlignXMap[type];
    }
    return gridNode::eGridAlignX::TOP;
}

gridNode::eGridAlignY gridNode::getGridAlignYByString(const std::string& type) {
    if (gridAlignYMap.find(type) != gridAlignYMap.end()) {
        return gridAlignYMap[type];
    }
    return gridNode::eGridAlignY::LEFT;
}

void gridNode::updateGridTransform() {
    auto startPos = getPosition();
    auto containerSize = getContentSize();
    cocos2d::Vec2 pos = { static_cast<float>(paddingX.first), static_cast<float>(paddingX.second) };
    pos.y -= marginY.first;
    pos.x += marginX.first;
    for (const auto& child : getChildren()) {
        auto childSize = child->getContentSize() * child->getScale();
        child->setPosition(pos);
        if (direction == eGridDirection::HORIZONTAL) {
            pos.x += childSize.width + paddingX.first + paddingX.second;
            containerSize.width += childSize.width + paddingX.first + paddingX.second;
            if (containerSize.height < childSize.height) {
                containerSize.height = childSize.height;
            }
        } else if (direction == eGridDirection::VERTICAL) {
            pos.y -= childSize.height + paddingY.first + paddingY.second;
            containerSize.height += childSize.height + paddingY.first + paddingY.second;
            if (containerSize.width < childSize.width) {
                containerSize.width = childSize.width;
            }
        }
    }
    containerSize.height += marginY.second;
    containerSize.width += marginX.second;
    setContentSize(containerSize);
}
