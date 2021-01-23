#include "spriteNode.h"
#include "common/coreModule/nodes/spritePolygon.h"

using namespace common::coreModule;
using namespace cocos2d;

Sprite *spriteNode::createSpritePolygon(const std::string& filepath, float jd) {
	//Create a sprite
	auto sprite = Sprite::create(filepath);
	//Create an object that automatically obtains the shape
	auto polyInfo = spritePolygon(filepath);
	//Really get the shape of the rectangle
	cocos2d::Size size1 = cocos2d::Size(polyInfo.getWidth(), polyInfo.getHeight());
	cocos2d::Rect realRect2 = cocos2d::Rect(0, 0, size1.width, size1.height);
	//Get the shape point by calling the method
	std::vector<Vec2> dd = polyInfo.trace(realRect2, jd);
	//Filter filter points
	dd = polyInfo.reduce(dd, realRect2, jd);
	dd = polyInfo.expand(dd, realRect2, jd);
	//Create physical shape
	cocos2d::PhysicsBody* geo;
	geo = PhysicsBody::createPolygon(&dd[0], dd.size(), PHYSICSBODY_MATERIAL_DEFAULT, Vec2(-sprite->getContentSize().width / 2, -sprite->getContentSize().height / 2));
	//Set the offset position of the shape
	geo->setPositionOffset(Vec2(0, 0));
	//Set the physical properties of the sprite and return the sprite
	sprite->setPhysicsBody(geo);
	return sprite;
}
