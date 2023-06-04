#include "colorProperty.h"
#include "generic/utilityModule/logManager.h"
#include "generic/utilityModule/stringUtility.h"

using namespace generic::coreModule;


void colorProperty::parseProperty(ax::Node* node, const jsonObject& object) {
    if (object.HasMember("color") && object["color"].IsArray()) {
        auto color = object["color"].GetArray();
        if (color.Size() >= 3u && color.Size() <= 4u) {
            ax::Color3B rgb;
            rgb.r = static_cast<uint8_t>(color[0].GetFloat());
            rgb.g = static_cast<uint8_t>(color[1].GetFloat());
            rgb.b = static_cast<uint8_t>(color[2].GetFloat());
            node->setColor(rgb);
            if (color.Size() == 4u) {
                node->setOpacity(static_cast<uint8_t>(color[3].GetFloat()));
            }
        } else {
            LOG_ERROR("Node '{}' no has color property", propertyName.c_str());
        }
    }
}
