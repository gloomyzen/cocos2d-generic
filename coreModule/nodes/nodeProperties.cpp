#include "nodeProperties.h"
#include "generic/utilityModule/stringUtility.h"
#include <cassert>

using namespace generic::coreModule;
using namespace cocos2d;

std::string nodeProperties::defaultNodesPath = "properties/nodes/";

nodeProperties::~nodeProperties() {
    removeJsonData();
    removeSettingsData();
}

void nodeProperties::initWithProperties(const std::string& path, cocos2d::Node* node) {
    if (!node) {
        node = dynamic_cast<cocos2d::Node*>(this);
        if (!node) {
            LOG_ERROR("Node is null!");
            return;
        }
    }
    if (node->getName().empty()) {
        LOG_ERROR("Node has no identifier!");
        return;
    }
    loadJson(path);

    if (propertyData.HasParseError() || !propertyData.IsObject()) {
        LOG_ERROR(CSTRING_FORMAT("Json file '%s' for node '%s' has errors or not found!", pathProperties.c_str(), node->getName().c_str()));
        return;
    }
    if (propertyData.HasMember("struct") && propertyData["struct"].IsObject()) {
        auto strObject = propertyData["struct"].GetObject();
        if (!strObject.HasMember("type") || !strObject.HasMember("name") || !strObject["type"].IsString()
            || !strObject["name"].IsString()) {
            LOG_ERROR(
              CSTRING_FORMAT("Json file '%s' for node '%s' not has 'type' and 'name'!", pathProperties.c_str(), node->getName().c_str()));
            return;
        }

        /// If the first named element is different from the current node, add the new node as a child to the
        /// current node
        cocos2d::Node* usedNode;
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

        parseProperties(usedNode, usedNode->getName());
    }
}

void nodeProperties::loadProperty(cocos2d::Node* node, const std::string& name) {
    if (node == nullptr) {
        return;
    }
    if (node->getName().empty()) {
        LOG_ERROR("Node has no identifier!");
        return;
    }
    parseProperties(node, name);
}

void nodeProperties::removeJsonData() {
    if (!propertyData.HasParseError() && propertyData.IsObject()) {
        propertyData.RemoveAllMembers();
    }
    pathProperties.clear();
}

void nodeProperties::loadJson(const std::string& path) {
    removeJsonData();
    pathProperties = STRING_FORMAT("%s%s", defaultNodesPath.c_str(), path.c_str());
    propertyData = GET_JSON(pathProperties);
}

void nodeProperties::parseProperties(cocos2d::Node* node, const std::string& name) {
    if (propertyData.HasParseError() || !propertyData.IsObject()) {
        LOG_ERROR(CSTRING_FORMAT("Json file '%s' contains errors or not found!", pathProperties.c_str()));
        return;
    }
    if (!propertyData.HasMember("props") || !propertyData["props"].IsObject()) {
        LOG_ERROR(CSTRING_FORMAT("Json file '%s' don't have 'props' object!", pathProperties.c_str()));
        return;
    }
    auto propJson = propertyData["props"].GetObject();
    auto nodeName = !name.empty() ? name : node->getName();
    if (propJson.HasMember(nodeName.c_str()) && propJson[nodeName.c_str()].IsObject()) {
        auto propObj = propJson[nodeName.c_str()].GetObject();
        for (const auto& propertyName : GET_NODE_FACTORY().getPropertiesPriority()) {
            if (propertyName.empty()) {
                LOG_ERROR(CSTRING_FORMAT("Bad property '%s' in 'propertyPriorityList'", propertyName.c_str()));
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

void nodeProperties::parseData(cocos2d::Node* node, const rapidjson::GenericValue<rapidjson::UTF8<char>>::Array& array) {
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
            parseProperties(childNode, childNode->getName());
            if (item.HasMember("child")) {
                parseData(childNode, item["child"].GetArray());
            }
        }
    }
}

const jsonObject& nodeProperties::getPropertyObject(const std::string& name) {
    assert(hasPropertyObject(name) && "Can't find property, use method 'hasPropertyObject' first!");

    auto obj = propertyData["props"].FindMember(name.c_str());
    static auto result = obj->value.GetObject();
    return result;
}

const jsonArray& nodeProperties::getPropertyArray(const std::string& name) {
    assert(hasPropertyArray(name) && "Can't find property, use method 'hasPropertyArray' first!");

    auto obj = propertyData["props"].FindMember(name.c_str());
    static auto result = obj->value.GetArray();
    return result;
}

bool nodeProperties::hasPropertyObject(const std::string& name) const {
    if (propertyData.HasParseError() || !propertyData.IsObject()) {
        return false;
    }
    if (!propertyData.HasMember("props") || !propertyData["props"].IsObject()) {
        return false;
    }
    if (propertyData["props"].HasMember(name.c_str())) {
        return propertyData["props"].GetObject()[name.c_str()].IsObject();
    }
    return false;
}

bool nodeProperties::hasPropertyArray(const std::string& name) const {
    if (propertyData.HasParseError() || !propertyData.IsObject()) {
        return false;
    }
    if (!propertyData.HasMember("props") || !propertyData["props"].IsObject()) {
        return false;
    }
    if (propertyData["props"].HasMember(name.c_str())) {
        return propertyData["props"].GetObject()[name.c_str()].IsArray();
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
