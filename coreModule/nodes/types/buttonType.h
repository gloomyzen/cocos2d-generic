#ifndef GENERIC_BUTTONTYPE_H
#define GENERIC_BUTTONTYPE_H

#include "cocos2d.h"
#include "generic/coreModule/nodes/types/eventNode.h"
#include "generic/coreModule/scenes/scenesFactoryInstance.h"
#include "generic/coreModule/scenes/sceneInterface.h"
#include <functional>
#include <string>

namespace generic::coreModule {

    template<typename T>
    class buttonType
      : public T
      , public generic::coreModule::eventNode {

        enum class eButtonStatus {
            START_CLICK = 0,
            END_CLICK,
        };

    public:

        ~buttonType() {}
        void initListener() {
            if (auto node = dynamic_cast<cocos2d::Node*>(this)) {
                node->setCascadeColorEnabled(true);
            }
            listener = cocos2d::EventListenerTouchOneByOne::create();
            listener->setSwallowTouches(true);
            listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event){
                if (hasShapePhysicsBody()) {
                    auto body = getShapePhysicsBody();
                    auto touchPos = touch->getStartLocation();
                    auto shapes = body->getShapes();
                    auto find = std::find_if(shapes.begin(), shapes.end(), [&](cocos2d::PhysicsShape* shape){
                        return shape->containsPoint(touchPos);
                    });
                    if (!shapes.empty() && find == shapes.end()) {
                        return false;
                    }
                } else {
                    auto node = event->getCurrentTarget();
                    auto touchLocation = node->convertToNodeSpace(touch->getLocation());
                    bool correctNode = node->getBoundingBox().containsPoint(touchLocation);
                    if (!correctNode)
                        return false;
                }
                return true;
            };
            listener->onTouchMoved = [this](cocos2d::Touch* touch, cocos2d::Event* event){
                return true;
            };
            listener->onTouchCancelled = listener->onTouchEnded = [this](cocos2d::Touch* touch, cocos2d::Event* event){
                return true;
            };
            GET_CURRENT_SCENE()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
        }
        bool getAllowSpamTap() const { return allowSpamTap; }
        void setAllowSpamTap(bool value) { allowSpamTap = value; }
        bool getAllowClick() const { return allowClick; }
        void setAllowClick(bool value) { allowClick = value; }
        void setChangeColorByClick(bool value) { changeColorByClick = value; }
        bool hasShapePhysicsBody() {
            if (auto node = dynamic_cast<cocos2d::Node*>(this)) {
                return node->getPhysicsBody() != nullptr;
            }
            return false;
        }
        cocos2d::PhysicsBody* getShapePhysicsBody() {
            if (auto node = dynamic_cast<cocos2d::Node*>(this)) {
                return node->getPhysicsBody();
            }
            return nullptr;
        }

    private:
        cocos2d::EventListenerTouchOneByOne* listener = nullptr;
        bool allowSpamTap = false;
        bool allowClick = true;
        bool changeColorByClick = true;

    protected:
        cocos2d::Color3B defaultColor;
        int moveTimes = 0;
    };
}


#endif// GENERIC_BUTTONTYPE_H
