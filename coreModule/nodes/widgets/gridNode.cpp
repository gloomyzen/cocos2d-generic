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
			if (x >= columns && y <= rows) {
				x = 0;
				y++;
			} else if (y >= rows && x <= columns) {
				y = 0;
				x++;
			}
			auto colIt = tempCols.find(x);
			auto rowIt = tempRows.find(y);
			auto childSize = child->getContentSize() * child->getScale();
			childSize.width += paddingX.first + paddingX.second;
			childSize.height += paddingY.first + paddingY.second;
			grid[y][x] = new sGridCell(child, childSize);
			if (colIt != tempCols.end()) {
				if (tempCols[x]->size.width < childSize.width) {
					tempCols[x]->size.width = childSize.width;
				}
			} else {
				tempCols[x] = new sGridCell(nullptr, childSize);
			}
			if (rowIt != tempRows.end()) {
				if (tempRows[y]->size.height < childSize.height) {
					tempRows[y]->size.height = childSize.height;
				}
			} else {
				tempRows[y] = new sGridCell(nullptr, childSize);
			}
			if (x < columns) {
				x++;
			} else {
				y++;
			}
//			if (direction == eGridDirection::HORIZONTAL) {
//				y++;
//			} else if (direction == eGridDirection::VERTICAL) {
//				x++;
//			}
		}
	}

	auto startPos = getPosition();
	auto containerSize = getContentSize();
	for (auto y = 0; y < grid.size(); ++y) {
		auto tempPos = startPos;
		for (auto x = 0; x < grid[y].size(); ++x) {
			auto width = tempCols[x]->size.width;
			auto height = tempRows[y]->size.height;
			if (x == 0) {
				startPos.y += height;
				containerSize.height += height;
			}
			if (y == 0) {
				containerSize.width += width;
			}

			if (grid[y][x]->node != nullptr) {
				auto cellPos = tempPos;
				if (x == 0) {
					cellPos.x += paddingX.first;
				}
				grid[y][x]->node->setPosition(cellPos);
			}
			tempPos.y += height;
			tempPos.x += width;
		}
	}
	setContentSize(containerSize);
}

