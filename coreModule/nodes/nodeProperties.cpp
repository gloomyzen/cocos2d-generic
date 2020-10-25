#include "nodeProperties.h"
#include "coreModule/resources/resourceManager.h"
#include "coreModule/nodes/nodeFactory.h"
#include "coreModule/gameManager.h"

using namespace mb::coreModule;
using namespace cocos2d;
using namespace rapidjson;


void nodeProperties::loadProperty(const std::string &path, Node *node) {
	if (node->getName().empty()) {
		LOG_ERROR("Node::loadProperty Node has no identifier!");
		return;
	}
	std::string pathNodes = "nodes/" + path;
	std::string pathProperties = "properties/" + path;

	auto json = GET_JSON(pathNodes);
	if (json.HasParseError() || !json.IsObject()) {
		LOG_ERROR("nodeProperties::loadProperty Json file '" + pathNodes + "' has erroes or not found!");
		return;
	}
	if (!json.HasMember("type") || !json.HasMember("name") || !json["type"].IsString() || !json["name"].IsString()) {
		LOG_ERROR("nodeProperties::loadProperty Json file '" + pathNodes + "' not has 'type' and 'name'!");
		return;
	}

	/// If the first named element is different from the current node, add the new node as a child to the current node
	Node* usedNode;
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

	parseComponents(usedNode, pathProperties, true);
}

void nodeProperties::parseData(Node *node, const GenericValue<UTF8<char>>::Array &array) {
	for (auto &item : array) {
		if (item["type"].IsString() && item["name"].IsString()) {
			auto childNode = GET_NODE_FACTORY().createNodeWithType(item["type"].GetString());
			childNode->setName(item["name"].GetString());
			if (item.HasMember("child")) {
				parseData(childNode, item["child"].GetArray());
			}
			node->addChild(childNode);
		}
	}
}

void nodeProperties::parseComponents(Node *node, const std::string &pathProperties, bool recursive) {
	auto propJson = GET_JSON(pathProperties);

	if (propJson.HasParseError() || !propJson.IsObject()) {
		LOG_ERROR("nodeProperties::parseProperty Json file '" + pathProperties + "' has errors or not found!");
		return;
	}
	bool found = false;
	for (auto &propList : propJson.GetObject()) {
		auto nodeName = propList.name.GetString();
		if (!recursive && nodeName != node->getName()) {
			continue;
		} else if (!recursive && nodeName == node->getName()) {
			found = true;
		}
		if (!propJson[nodeName].IsObject()) {
			continue;
		}
		auto *targetNode = node->findNode(nodeName, node);
		if (targetNode == nullptr) continue;

		for (const auto &component : componentPriorityList) {
			if (component.empty()) {
				LOG_ERROR(StringUtils::format("nodeProperties::parseProperty bad property '%s' in 'componentPriorityList'", component.c_str()));
				continue;
			}
			const auto componentItr = propJson[nodeName].FindMember(component.c_str());
			if(componentItr != propJson[nodeName].MemberEnd()) {
				if (componentItr->value.IsObject()) {
					GET_NODE_FACTORY().getComponents(targetNode, component, componentItr->value.GetObject());
				}
			}
		}

		if (found && !recursive) {
			return;
		}
	}
}
