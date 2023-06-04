#include "transformProperty.h"
#include "generic/utilityModule/logManager.h"
#include "generic/utilityModule/stringUtility.h"
#include "generic/coreModule/components/transformComponent.h"

using namespace generic::coreModule;


void baseProperty::parseProperty(ax::Node* node, const jsonObject& object) {
    if (object.HasMember("position")) {
        auto positions = object["position"].GetArray();
        if (positions.Size() == 2u) {
            node->setPosition(positions[0].GetFloat(), positions[1].GetFloat());
        } else if (positions.Size() == 3u) {
            node->setPosition3D(ax::Vec3(positions[0].GetFloat(), positions[1].GetFloat(), positions[2].GetFloat()));
        } else {
            LOG_ERROR("Property '{}' has wrong '{}' position keys", propertyName.c_str(), positions.Size());
        }
    }
    if (object.HasMember("pivot")) {
        auto pivot = object["pivot"].GetArray();
        if (pivot.Size() == 2u) {
            auto component = dynamic_cast<transformComponent*>(node->getComponent(transformComponent::TRANSFORM_COMPONENT_NAME));
            if (!component) {
                component = new transformComponent();
                node->addComponent(component);
            }
            component->setPivotPoint(ax::Vec2(pivot[0].GetFloat(), pivot[1].GetFloat()));
        } else {
            LOG_ERROR("Property '{}' has wrong '{}' pivot keys", propertyName.c_str(), pivot.Size());
        }
    }
    if (object.HasMember("anchor")) {
        auto anchor = object["anchor"].GetArray();
        if (anchor.Size() == 2u) {
            node->setAnchorPoint(ax::Vec2(anchor[0].GetFloat(), anchor[1].GetFloat()));
        } else {
            LOG_ERROR("Property '{}' has wrong '{}' anchor keys", propertyName.c_str(), anchor.Size());
        }
    }
    if (object.HasMember("size")) {
        auto size = object["size"].GetArray();
        if (size.Size() == 2u) {
            bool onlyScaleMode = false;
            float scaleX, scaleY;
            scaleX = scaleY = .0f;
            if (auto sprite = dynamic_cast<ax::Sprite*>(node)) {
//                if (sprite->getRenderMode() == ax::Sprite::RenderMode::QUAD_BATCHNODE
//                    || sprite->getRenderMode() == ax::Sprite::RenderMode::POLYGON) {
                    onlyScaleMode = true;
                    const auto& content = sprite->getContentSize();
                    auto _size = ax::Size();
                    _size.width = size[0].GetFloat();
                    _size.height = size[1].GetFloat();
                    if (content.width != 0.f) {
                        scaleX = _size.width / content.width;
                        scaleY = _size.height / content.height;
                    }
//                }
            }
            if (onlyScaleMode) {
                node->setScaleX(scaleX);
                node->setScaleY(scaleY);
            } else {
                auto _size = ax::Size();
                _size.width = size[0].GetFloat();
                _size.height = size[1].GetFloat();
                node->setContentSize(_size);
            }
        } else {
            LOG_ERROR("Property '{}' has wrong '{}' size keys", propertyName.c_str(), size.Size());
        }
    }
    if (object.HasMember("scale") && object["scale"].IsNumber()) {
        auto scale = object["scale"].GetFloat();
        node->setScale(scale);
    } else if (object.HasMember("scale") && object["scale"].IsArray()) {
        auto scale = object["scale"].GetArray();
        if (scale.Size() == 2u) {
            node->setScale(scale[0].GetFloat(), scale[1].GetFloat());
        } else {
            LOG_ERROR("Property '{}' has wrong '{}' scale keys", propertyName.c_str(), scale.Size());
        }
    }
    if (object.HasMember("rotation") && object["rotation"].IsNumber()) {
        node->setRotation(object["rotation"].GetFloat());
    }
    if (object.HasMember("rotation3d")) {
        auto rotation3d = object["rotation3d"].GetArray();
        if (rotation3d.Size() == 3u) {
            node->setRotation3D(ax::Vec3(rotation3d[0].GetFloat(), rotation3d[1].GetFloat(), rotation3d[2].GetFloat()));
        } else {
            LOG_ERROR("Property '{}' has wrong '{}' rotation3d keys", propertyName.c_str(), rotation3d.Size());
        }
    }
    if (object.HasMember("stretch")) {
        auto stretch = object["stretch"].GetArray();
        if (stretch.Size() == 2u) {
            auto component = dynamic_cast<transformComponent*>(node->getComponent(transformComponent::TRANSFORM_COMPONENT_NAME));
            if (!component) {
                component = new transformComponent();
                node->addComponent(component);
            }
            component->setStretch(stretch[0].GetFloat(), stretch[1].GetFloat());
        } else {
            LOG_ERROR("Property '{}' has wrong '{}' stretch keys", propertyName.c_str(), stretch.Size());
        }
    }
}
