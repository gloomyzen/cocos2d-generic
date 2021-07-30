#ifndef GENERIC_NODEFACTORY_H
#define GENERIC_NODEFACTORY_H

#include "cocos2d.h"
#include "generic/utilityModule/jsonHelper.h"
#include <functional>
#include <map>
#include <string>

#define GET_NODE_FACTORY() generic::coreModule::nodeFactory::getInstance()

namespace generic {
    namespace coreModule {
        using namespace cocos2d;

        enum class eNodeFactory {
            TRANSFORM_COMPONENT,
            SPRITE_COMPONENT,
            LABEL_COMPONENT,
            DRAGONBONES_COMPONENT,
            SPINE_COMPONENT,
            COLOR_COMPONENT,
            SCROLL_VIEW_COMPONENT,
            GRID_COMPONENT,
            SCALE9SPRITE_COMPONENT
        };

        static const std::vector<std::string> componentPriorityList = {
            { "spriteComponent" },    { "labelComponent" }, { "dragonbonesComponent" }, { "spineComponent" }, { "scale9spriteComponent" },
            { "transformComponent" }, { "colorComponent" }, { "scrollViewComponent" },  { "gridComponent" }
        };

        class nodeFactory {
        public:
            nodeFactory();
            ~nodeFactory();
            static nodeFactory& getInstance();

            void getComponents(Node* node,
                               const std::string& componentName,
                               const rapidjson::GenericValue<rapidjson::UTF8<char>>::Object& object);

            bool hasRegisteredComponent(const std::string& componentName);

            Node* createNodeWithType(const std::string& type);

            bool registerCustomNodeType(const std::string&, std::function<Node*()>);

        private:
            void init();
            bool inited = false;
        };
    }// namespace coreModule
}// namespace generic


#endif// GENERIC_NODEFACTORY_H
