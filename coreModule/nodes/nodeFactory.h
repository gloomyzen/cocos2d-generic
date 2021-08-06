#ifndef GENERIC_NODEFACTORY_H
#define GENERIC_NODEFACTORY_H

#include "cocos2d.h"
#include "generic/utilityModule/jsonHelper.h"
#include <functional>
#include <map>
#include <string>

#define GET_NODE_FACTORY() generic::coreModule::nodeFactory::getInstance()

namespace generic::coreModule {

    enum class eNodeFactory {
        TRANSFORM_COMPONENT = 0,
        SPRITE_COMPONENT,
        LABEL_COMPONENT,
        DRAGONBONES_COMPONENT,
        SPINE_COMPONENT,
        COLOR_COMPONENT,
        SCROLL_VIEW_COMPONENT,
        GRID_COMPONENT,
        SCALE9SPRITE_COMPONENT,
        CLIP_COMPONENT
    };

    static const std::vector<std::string> componentPriorityList = {
        { "spriteComponent" },    { "labelComponent" }, { "dragonbonesComponent" }, { "spineComponent" }, { "scale9spriteComponent" },
        { "transformComponent" }, { "colorComponent" }, { "scrollViewComponent" },  { "gridComponent" }, { "clipComponent" }
    };

    class nodeFactory {
    public:
        nodeFactory();
        ~nodeFactory();
        static nodeFactory& getInstance();
        void cleanup();

        void readComponent(cocos2d::Node* node,
                           const std::string& componentName,
                           const rapidjson::GenericValue<rapidjson::UTF8<char>>::Object& component);

        static bool hasRegisteredComponent(const std::string& componentName);

        cocos2d::Node* createNodeWithType(const std::string& type);

        bool registerCustomNodeType(const std::string&, std::function<cocos2d::Node*()>);

    private:
        void init();
        bool inited = false;
        std::map<std::string, std::function<cocos2d::Node*()>> nodes;
    };
}// namespace generic::coreModule


#endif// GENERIC_NODEFACTORY_H
