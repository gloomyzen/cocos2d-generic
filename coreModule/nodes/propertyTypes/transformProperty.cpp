#include "transformProperty.h"
#include "generic/utilityModule/stringUtility.h"
#include "generic/debugModule/logManager.h"

using namespace generic::coreModule;


bool transformProperty::parseProperty(cocos2d::Node* node, const jsonObject& object) {
    if (object.HasMember("position")) {
        auto positions = object["position"].GetArray();
        if (positions.Size() == 2) {
            node->setPosition(positions[0].GetFloat(), positions[1].GetFloat());
        } else if (positions.Size() == 3) {
            node->setPosition3D(cocos2d::Vec3(positions[0].GetFloat(), positions[1].GetFloat(), positions[2].GetFloat()));
        } else {
            LOG_ERROR(CSTRING_FORMAT("Component '%s' has wrong '%s' position keys!",
                                     propertyName.c_str(),
                                     std::to_string(positions.Size()).c_str()));
        }
    }
    if (object.HasMember("pivot")) {
        auto pivot = object["pivot"].GetArray();
        if (pivot.Size() == 2) {
            node->setPivotPoint(cocos2d::Vec2(pivot[0].GetFloat(), pivot[1].GetFloat()));
        } else {
            LOG_ERROR(CSTRING_FORMAT("Component '%s' has wrong '%s' pivot keys!",
                                     propertyName.c_str(),
                                     std::to_string(pivot.Size()).c_str()));
        }
    }
    if (object.HasMember("anchor")) {
        auto anchor = object["anchor"].GetArray();
        if (anchor.Size() == 2) {
            node->setAnchorPoint(cocos2d::Vec2(anchor[0].GetFloat(), anchor[1].GetFloat()));
        } else {
            LOG_ERROR(CSTRING_FORMAT("Component '%s' has wrong '%s' anchor keys!",
                                     propertyName.c_str(),
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
                                     propertyName.c_str(),
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
                                     propertyName.c_str(),
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
                                     propertyName.c_str(),
                                     std::to_string(rotation3d.Size()).c_str()));
        }
    }
    if (object.HasMember("stretch")) {
        auto stretch = object["stretch"].GetArray();
        if (stretch.Size() == 2) {
            node->setStretch(stretch[0].GetFloat(), stretch[1].GetFloat());
        } else {
            LOG_ERROR(CSTRING_FORMAT("Component '%s' has wrong '%s' stretch keys!",
                                     propertyName.c_str(),
                                     std::to_string(stretch.Size()).c_str()));
        }
    }
    return false;
}
