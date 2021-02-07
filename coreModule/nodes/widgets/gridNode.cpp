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
	gridTable grid;
	gridCells tempCols;
	gridCells tempRows;

	{ //update temp grid for calculate width and height for grid
		auto x = 0;
		auto y = 0;
		for (const auto& child : getChildren()) {
			if (y > columns && direction == eGridDirection::VERTICAL){
				x = 0;
			}
			if (x > rows && direction == eGridDirection::HORIZONTAL) {
				y = 0;
			}
			auto colIt = tempCols.find(y);
			auto rowIt = tempRows.find(x);
			grid[y][x] = new sGridCell(child, child->getContentSize());
			if (colIt != tempCols.end()) {
				if (tempCols[y]->size.width < child->getContentSize().width) {
					tempCols[y]->size.width = child->getContentSize().width;
				}
			} else {
				tempCols[y] = new sGridCell(nullptr, child->getContentSize());
			}
			if (rowIt != tempRows.end()) {
				if (tempRows[x]->size.height < child->getContentSize().height) {
					tempRows[x]->size.height = child->getContentSize().height;
				}
			} else {
				tempRows[x] = new sGridCell(nullptr, child->getContentSize());
			}
			if (direction == eGridDirection::HORIZONTAL) {
				y++;
			} else if (direction == eGridDirection::VERTICAL) {
				x++;
			}
		}
	}

	auto startPos = getPosition();
	for (auto y = 0; y < grid.size(); ++y) {
		for (auto x = 0; x < grid[y].size(); ++x) {
//			if (alignY == eGridAlignY::RIGHT) {
//				//
//			} else if (alignY == eGridAlignY::LEFT) {
//				//
//			}
			auto width = tempCols[y]->size.width;
			auto height = tempRows[x]->size.height;
//			startPos.x =
			if (grid[y][x]->node != nullptr) {
				//
			}
		}
	}
}

