#ifndef GENERIC_NODEFACTORY_H
#define GENERIC_NODEFACTORY_H

#include "cocos2d.h"
#include "generic/coreModule/nodes/propertyTypes/propertyInterface.h"
#include "generic/utilityModule/jsonHelper.h"
#include <functional>
#include <map>
#include <string>

#define GET_NODE_FACTORY() generic::coreModule::nodeFactory::getInstance()

namespace generic::coreModule {

    const std::vector<std::string> propertyPriorityList = {
        { "spriteProperty" },    { "labelProperty" }, { "dragonbonesProperty" }, { "spineProperty" }, { "scale9SpriteProperty" },
        { "transformProperty" }, { "colorProperty" }, { "scrollViewProperty" },  { "gridProperty" },  { "clipProperty" }
    };

    class nodeFactory {
    public:
        nodeFactory();
        ~nodeFactory();
        static nodeFactory& getInstance();
        void cleanup();

        void readProperty(cocos2d::Node* node,
                           const std::string& propertyName,
                           const jsonObject& json);

        bool hasRegisteredProperty(const std::string& propertyName);

        cocos2d::Node* createNodeWithType(const std::string& type);

        bool registerCustomNodeType(const std::string&, std::function<cocos2d::Node*()>);

    private:
        void init();
        bool inited = false;
        std::map<std::string, std::function<cocos2d::Node*()>> nodes;
        std::map<std::string, propertyInterface*> propertiesMap;
    };
}// namespace generic::coreModule


#endif// GENERIC_NODEFACTORY_H
