#include "gridNode.h"

using namespace common::coreModule;

static std::map<std::string, gridNode::eGridDirection> gridDirectionMap = {
		{"horizontal", gridNode::eGridDirection::HORIZONTAL},
		{"vertical", gridNode::eGridDirection::VERTICAL},
};
static std::map<std::string, gridNode::eGridAlignX> gridAlignXMap = {
		{"top", gridNode::eGridAlignX::TOP},
		{"bottom", gridNode::eGridAlignX::BOTTOM},
};

static std::map<std::string, gridNode::eGridAlignY> gridAlignYMap = {
		{"left", gridNode::eGridAlignY::LEFT},
		{"right", gridNode::eGridAlignY::RIGHT},
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
	gridType grid;
	auto currentCol = 0;
	auto currentRow = 0;
	for (const auto& child : getChildren()) {
		if (currentCol > columns && direction == eGridDirection::VERTICAL){
			currentRow = 0;
		}
		if (currentRow > rows && direction == eGridDirection::HORIZONTAL) {
			currentCol = 0;
		}
		grid[currentCol][currentRow] = child;
		if (direction == eGridDirection::HORIZONTAL) {
			currentCol++;
		} else if (direction == eGridDirection::VERTICAL) {
			currentRow++;
		}
	}
	//0 0
	//0 0
	auto startPos = getPosition();
	for (auto itemRow = 0; itemRow < grid.size(); ++itemRow) {
		for (auto itemCol = 0; itemCol < grid[itemRow].size(); ++itemCol) {
			if (alignY == eGridAlignY::RIGHT) {
				//
			} else if (alignY == eGridAlignY::LEFT) {
				//
			}
			//
		}
	}
}

