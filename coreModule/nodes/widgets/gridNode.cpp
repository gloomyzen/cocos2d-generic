#include "gridNode.h"

using namespace common::coreModule;

gridNode::gridNode() {
	marginX = {0, 0};
	marginY = {0, 0};
	paddingX = {0, 0};
	paddingY = {0, 0};
}

gridNode::~gridNode() {}

void gridNode::addChild(Node *child) {
	Node::addChild(child);
	updateTransform();
}

void gridNode::updateTransform() {
	for( const auto &child: _children)
		child->updateTransform();
}

