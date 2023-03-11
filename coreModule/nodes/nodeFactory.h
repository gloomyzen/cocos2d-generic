#pragma once
#ifndef GENERIC_NODEFACTORY_H
#define GENERIC_NODEFACTORY_H

#include "axmol.h"
#include "generic/coreModule/nodes/propertyTypes/propertyInterface.h"
#include "generic/utilityModule/jsonHelper.h"
#include <functional>
#include <map>
#include <vector>
#include <string>

#define GET_NODE_FACTORY() generic::coreModule::nodeFactory::getInstance()

namespace generic::coreModule {

    class nodeFactory {
    public:
        ~nodeFactory();
        static nodeFactory& getInstance();
        static void cleanup();

        void readProperty(ax::Node* node, const std::string& propertyName, const jsonObject& json);
        bool hasRegisteredProperty(const std::string& propertyName);
        ax::Node* createNodeWithType(const std::string& type);
        bool registerCustomNodeType(const std::string&, std::function<ax::Node*()>);
        const std::vector<std::string>& getPropertiesPriority() { return propertyPriorityList; }
    private:
        nodeFactory();
        nodeFactory(const nodeFactory&) = default;
        nodeFactory& operator=(const nodeFactory&) = default;
        static void create();
        static void onDeadReference();

        static nodeFactory* pInstance;
        static bool destroyed;

        std::map<std::string, std::function<ax::Node*()>> nodes;
        std::map<std::string, propertyInterface*> propertiesMap;
        std::vector<std::string> propertyPriorityList;
    };
}// namespace generic::coreModule


#endif// GENERIC_NODEFACTORY_H
