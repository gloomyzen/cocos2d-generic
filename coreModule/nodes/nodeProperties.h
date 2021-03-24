#ifndef COMMON_NODEPROPERTIES_H
#define COMMON_NODEPROPERTIES_H

#include "cocos2d.h"
#include "common/coreModule/gameManager.h"
#include "common/coreModule/nodes/nodeFactory.h"
#include "common/coreModule/resources/resourceManager.h"
#include "common/coreModule/resources/settings/settingManager.h"
#include "common/debugModule/logManager.h"
#include "json/document.h"
#include <string>

namespace common::coreModule {

    template<typename T>
    class nodeProperties : public T {
      public:
        ~nodeProperties() {
            propertyData.EraseMember(propertyData.MemberBegin(), propertyData.MemberEnd());
            pathProperties.clear();
        }
        /***
         * Парсинг параметров из json файла
         * Полный парсинг нод и следом параметров каждой ноды
         * @param path relative path to file
         * @param node Node instance
         */
        void loadProperty(const std::string& path, cocos2d::Node* node = nullptr) {
            if (node == nullptr)
                node = this;
            if (node->getName().empty()) {
                LOG_ERROR("Node::loadProperty Node is null or node has no identifier!");
                return;
            }
            pathProperties = StringUtils::format("properties/nodeProperties/%s", path.c_str());
            propertyData = GET_JSON(pathProperties);
            std::string pathNodes = "properties/nodes/" + path;

            auto json = GET_JSON(pathNodes);
            if (json.HasParseError() || !json.IsObject()) {
                LOG_ERROR("nodeProperties::loadProperty Json file '" + pathNodes + "' has errors or not found!");
                return;
            }
            if (!json.HasMember("type") || !json.HasMember("name") || !json["type"].IsString()
                || !json["name"].IsString()) {
                LOG_ERROR("nodeProperties::loadProperty Json file '" + pathNodes + "' not has 'type' and 'name'!");
                return;
            }

            /// If the first named element is different from the current node, add the new node as a child to the
            /// current node
            cocos2d::Node* usedNode;
            if (json["name"].GetString() != node->getName()) {
                usedNode = GET_NODE_FACTORY().createNodeWithType(json["type"].GetString());
                usedNode->setName(json["name"].GetString());
                node->addChild(usedNode);
            } else {
                usedNode = node;
            }

            if (json.HasMember("child") && json["child"].IsArray()) {
                parseData(usedNode, json["child"].GetArray());
            }

            parseComponents(usedNode, usedNode->getName());
        }

        /***
         * Парсинг параметров из json файла
         * Парсинг только параметров одной ноды
         * @param node Node instance
         * @param name
         */
        void loadComponent(cocos2d::Node* node = nullptr, const std::string& name = std::string()) {
            if (node == nullptr) {
                node = this;
            }
            if (node->getName().empty()) {
                LOG_ERROR("Node::loadProperty Node has no identifier!");
                return;
            }
            parseComponents(node, name);
        }

        void removeJsonData() {
            propertyData.EraseMember(propertyData.MemberBegin(), propertyData.MemberEnd());
            pathProperties.clear();
        }

      private:

        void parseComponents(cocos2d::Node* node, const std::string& name = std::string()) {
            if (propertyData.HasParseError() || !propertyData.IsObject()) {
                LOG_ERROR(
                    StringUtils::format("nodeProperties::parseProperty Json file '%s' contains errors or not found!",
                                        pathProperties.c_str()));
                return;
            }
            auto nodeName = !name.empty() ? name : node->getName();
            auto propJson = propertyData.GetObjectJ();
            if (propJson.HasMember(nodeName.c_str()) && propJson[nodeName.c_str()].IsObject()) {
                auto propObj = propJson[nodeName.c_str()].GetObjectJ();
                for (const auto& component : componentPriorityList) {
                    if (component.empty()) {
                        LOG_ERROR(StringUtils::format(
                            "nodeProperties::parseProperty bad property '%s' in 'componentPriorityList'",
                            component.c_str()));
                        continue;
                    }
                    const auto componentItr = propObj.FindMember(component.c_str());
                    if (componentItr != propObj.MemberEnd()) {
                        if (componentItr->value.IsObject()) {
                            GET_NODE_FACTORY().getComponents(node, component, componentItr->value.GetObjectJ());
                        }
                    }
                }
            }
        }

        void parseData(cocos2d::Node* node, const rapidjson::GenericValue<rapidjson::UTF8<char>>::Array& array) {
            parseComponents(node, node->getName());
            for (auto& item : array) {
                if (item["type"].IsString() && item["name"].IsString()) {
                    auto childNode = GET_NODE_FACTORY().createNodeWithType(item["type"].GetString());
                    childNode->setName(item["name"].GetString());
                    if (item.HasMember("child")) {
                        parseData(childNode, item["child"].GetArray());
                    }
                    node->addChild(childNode);
                    parseComponents(childNode, childNode->getName());
                }
            }
        }

        rapidjson::Document propertyData;
        std::string pathProperties;

    };
}// namespace common::coreModule

#endif// COMMON_NODEPROPERTIES_H
