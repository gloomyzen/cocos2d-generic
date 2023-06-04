#include "clipProperty.h"
#include "generic/utilityModule/logManager.h"
#include "generic/utilityModule/stringUtility.h"
#include "generic/coreModule/nodes/types/drawNodeBase.h"

using namespace generic::coreModule;


void clipProperty::parseProperty(ax::Node* node, const jsonObject& object) {
    if (auto clipNode = dynamic_cast<ax::ClippingNode*>(node)) {
        bool inverted = false;
        bool autoUpdate = false;
        auto stencilColor = ax::Color4F::BLACK;
        if (object.HasMember("inverted") && object["inverted"].IsBool()) {
            inverted = object["inverted"].GetBool();
        }
        if (object.HasMember("autoUpdate") && object["autoUpdate"].IsBool()) {
            autoUpdate = object["autoUpdate"].GetBool();
        }
        if (object.HasMember("color") && object["color"].IsArray()) {
            auto color = object["color"].GetArray();
            if (color.Size() == 4u) {
                ax::Color4F rgba;
                rgba.r = color[0].GetFloat() / 255;
                rgba.g = color[1].GetFloat() / 255;
                rgba.b = color[2].GetFloat() / 255;
                rgba.a = color[3].GetFloat() / 255;
                stencilColor = rgba;
            }
        }
        //todo fix stencil by other external node with rotation scale etc
//        if (object.HasMember("nodeName") && object["nodeName"].IsString()) {
//            auto stencil = DrawNode::create();
//            auto name = object["nodeName"].GetString();
//            stencil->setName(name);
//            if (allProperties.HasMember(name) && allProperties[name].IsObject()) {
//                auto nodeObj = allProperties[name].GetObject();
//                if (nodeObj.HasMember("baseProperty") && nodeObj["baseProperty"].IsObject()) {
//                    auto componentObj = nodeObj["baseProperty"].GetObject();
//                    readComponent(stencil, "baseProperty", componentObj, allProperties);
//                }
//            }
//            clipNode->setInverted(inverted);
//            clipNode->addChild(stencil);
//            stencil->drawSolidRect(Vec2::ZERO, stencil->getContentSize(), stencilColor);
//            clipNode->setStencil(stencil);
//        } else {
//            LOG_ERROR("Component '{}' has invalid nodeName", componentName.c_str());
//        }
        auto stencil = ax::utils::createInstance<drawNodeBase>();
        stencil->drawSolidRect(ax::Vec2::ZERO, clipNode->getContentSize(), stencilColor);
        clipNode->setStencil(stencil);
//        clipNode->addChild(stencil);
        stencil->setDrawColor(stencilColor);
        stencil->setAutoUpdateEnabled(autoUpdate);
        stencil->forceUpdateRect();
        clipNode->setInverted(inverted);
    } else {
        LOG_ERROR("Node '{}' no has clip property", propertyName.c_str());
    }
}

