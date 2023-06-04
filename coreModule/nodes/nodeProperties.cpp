#include "nodeProperties.h"
#include "generic/utilityModule/stringUtility.h"
#include <cassert>
#include <fmt/core.h>

using namespace generic::coreModule;
using namespace ax;

std::string nodeProperties::defaultNodesPath = "properties/nodes/";

nodeProperties::~nodeProperties() {
    removeJsonData();
    removeSettingsData();
}

void nodeProperties::initWithProperties(const std::string_view& path, ax::Node* node) {
    if (!node) {
        node = dynamic_cast<ax::Node*>(this);
        if (!node) {
            LOG_ERROR("Node is null");
            return;
        }
    }
    if (node->getName().empty()) {
        LOG_ERROR("Node has no identifier");
        return;
    }
    loadJson(path);

    if (propertyData.HasParseError() || !propertyData.IsObject()) {
        LOG_ERROR("Json file '{}' for node '{}' has errors or not found", pathProperties.data(), node->getName().data());
        return;
    }
    if (propertyData.HasMember("struct") && propertyData["struct"].IsObject()) {
        auto strObject = propertyData["struct"].GetObject();
        if (!strObject.HasMember("type") || !strObject.HasMember("name") || !strObject["type"].IsString()
            || !strObject["name"].IsString()) {
            LOG_ERROR("Json file '{}' for node '{}' not has 'type' and 'name'", pathProperties.data(), node->getName().data());
            return;
        }

        /// If the first named element is different from the current node, add the new node as a child to the
        /// current node
        ax::Node* usedNode;
        if (strObject["name"].GetString() != node->getName()) {
            usedNode = GET_NODE_FACTORY().createNodeWithType(strObject["type"].GetString());
            usedNode->setName(strObject["name"].GetString());
            node->addChild(usedNode);
        } else {
            usedNode = node;
        }
        if (strObject.HasMember("settings") && strObject["settings"].IsObject()) {
            if (auto nodeWithProps = dynamic_cast<nodeProperties*>(usedNode)) {
                nodeWithProps->setSettingsData(strObject["settings"].GetObject());
                nodeWithProps->updateSettings();
                removeSettingsData();
            }
        }

        if (strObject.HasMember("child") && strObject["child"].IsArray()) {
            parseData(usedNode, strObject["child"].GetArray());
        }

        parseProperties(usedNode, usedNode->getName().data());
    }
}

void nodeProperties::loadProperty(ax::Node* node, const std::string_view& name) {
    if (node == nullptr) {
        return;
    }
    if (node->getName().empty()) {
        LOG_ERROR("Node has no identifier");
        return;
    }
    parseProperties(node, name);
}

void nodeProperties::removeJsonData() {
    if (!propertyData.HasParseError() && propertyData.IsObject()) {
        propertyData.RemoveAllMembers();
    }
    pathProperties = {};
}

void nodeProperties::loadJson(const std::string_view& path) {
    removeJsonData();
    pathProperties = fmt::format("{}{}", defaultNodesPath.data(), path.data());
    propertyData = GET_JSON(pathProperties.data());
}

void nodeProperties::parseProperties(ax::Node* node, const std::string_view& name) {
    if (propertyData.HasParseError() || !propertyData.IsObject()) {
        LOG_ERROR("Json file '{}' contains errors or not found", pathProperties.data());
        return;
    }
    if (!propertyData.HasMember("props") || !propertyData["props"].IsObject()) {
        LOG_ERROR("Json file '{}' don't have 'props' object", pathProperties.data());
        return;
    }
    auto propJson = propertyData["props"].GetObject();
    auto nodeName = !name.empty() ? name : node->getName();
    if (propJson.HasMember(nodeName.data()) && propJson[nodeName.data()].IsObject()) {
        auto propObj = propJson[nodeName.data()].GetObject();
        for (const auto& propertyName : GET_NODE_FACTORY().getPropertiesPriority()) {
            if (propertyName.empty()) {
                LOG_ERROR("Bad property '{}' in 'propertyPriorityList'", propertyName.c_str());
                continue;
            }
            const auto propertyItr = propObj.FindMember(propertyName.c_str());
            if (propertyItr != propObj.MemberEnd()) {
                if (propertyItr->value.IsObject()) {
                    GET_NODE_FACTORY().readProperty(node, propertyName, propertyItr->value.GetObject());
                }
            }
        }
    }
}

void nodeProperties::parseData(ax::Node* node, const rapidjson::GenericValue<rapidjson::UTF8<char>>::Array& array) {
    for (auto& item : array) {
        if (item["type"].IsString() && item["name"].IsString()) {
            auto childNode = GET_NODE_FACTORY().createNodeWithType(item["type"].GetString());
            childNode->setName(item["name"].GetString());
            if (item.HasMember("settings") && item["settings"].IsObject()) {
                if (auto nodeWithProps = dynamic_cast<nodeProperties*>(childNode)) {
                    nodeWithProps->setSettingsData(item["settings"].GetObject());
                    nodeWithProps->updateSettings();
                }
            }
            node->addChild(childNode);
            parseProperties(childNode, childNode->getName().data());
            if (item.HasMember("child")) {
                parseData(childNode, item["child"].GetArray());
            }
        }
    }
}

const jsonObject nodeProperties::getPropertyObject(const std::string_view& name) {
    assert(hasPropertyObject(name) && "Can't find property, use method 'hasPropertyObject' first!");

    auto obj = propertyData["props"].FindMember(name.data());
    auto result = obj->value.GetObject();
    return result;
}

const jsonArray nodeProperties::getPropertyArray(const std::string_view& name) {
    assert(hasPropertyArray(name) && "Can't find property, use method 'hasPropertyArray' first!");

    auto obj = propertyData["props"].FindMember(name.data());
    auto result = obj->value.GetArray();
    return result;
}

bool nodeProperties::hasPropertyObject(const std::string_view& name) const {
    if (propertyData.HasParseError() || !propertyData.IsObject()) {
        return false;
    }
    if (!propertyData.HasMember("props") || !propertyData["props"].IsObject()) {
        return false;
    }
    if (propertyData["props"].HasMember(name.data())) {
        return propertyData["props"].GetObject()[name.data()].IsObject();
    }
    return false;
}

bool nodeProperties::hasPropertyArray(const std::string_view& name) const {
    if (propertyData.HasParseError() || !propertyData.IsObject()) {
        return false;
    }
    if (!propertyData.HasMember("props") || !propertyData["props"].IsObject()) {
        return false;
    }
    if (propertyData["props"].HasMember(name.data())) {
        return propertyData["props"].GetObject()[name.data()].IsArray();
    }
    return false;
}

void nodeProperties::setSettingsData(const rapidjson::GenericValue<rapidjson::UTF8<char>>::Object& object) {
    settingsData = object;
}

rapidjson::Value nodeProperties::getSettingsData() {
    if (!settingsData.IsObject()) {
        rapidjson::Value data;
        data.SetObject();
        return data.GetObject();
    }
    return settingsData.GetObject();
}

void nodeProperties::removeSettingsData() {
    if (settingsData.IsObject()) {
        settingsData.RemoveAllMembers();
    }
}

void nodeProperties::updateSettings() {}
