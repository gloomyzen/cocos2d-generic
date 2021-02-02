#include "gridNode.h"

using namespace common::coreModule;

static std::map<std::string, gridNode::eGridDirection> gridDirectionMap = {
		{"horizontal", gridNode::eGridDirection::HORIZONTAL},
		{"vertical", gridNode::eGridDirection::VERTICAL},
};
static std::map<std::string, gridNode::eGridAlign> gridAlignMap = {
		{"left", gridNode::eGridAlign::LEFT},
		{"right", gridNode::eGridAlign::RIGHT},
		{"top", gridNode::eGridAlign::TOP},
		{"bottom", gridNode::eGridAlign::BOTTOM},
};

gridNode::gridNode() {
	marginX = {0, 0};
	marginY = {0, 0};
	paddingX = {0, 0};
	paddingY = {0, 0};
}

gridNode::~gridNode() {}

void gridNode::addChild(Node *child) {
	Node::addChild(child);
	updateGridTransform();
}

void gridNode::updateTransform() {
	for (const auto &child: _children)
		child->updateTransform();
	updateGridTransform();
}

gridNode::eGridDirection gridNode::getGridDirectionByString(const std::string& type) {
	if (gridDirectionMap.find(type) != gridDirectionMap.end()) {
		return gridDirectionMap[type];
	}
	return gridNode::eGridDirection::VERTICAL;
}

gridNode::eGridAlign gridNode::getGridAlignByString(const std::string& type) {
	if (gridAlignMap.find(type) != gridAlignMap.end()) {
		return gridAlignMap[type];
	}
	return gridNode::eGridAlign::LEFT;
}

void gridNode::updateGridTransform() {
	gridType grid;
	auto currentCol = 1;
	auto currentRow = 1;
	for (const auto& child : getChildren()) {
		if (currentCol > columns){
			if (direction == eGridDirection::HORIZONTAL) {
				//todo
			} else if (direction == eGridDirection::VERTICAL) {
				//todo
			}
		}
		if (currentRow > rows) {
			//
		}
		grid[currentCol][currentRow] = child;
		currentCol++;
		currentRow++;
	}
	if (direction == eGridDirection::HORIZONTAL) {
		//
	} else if (direction == eGridDirection::VERTICAL) {
		//
	}
}

