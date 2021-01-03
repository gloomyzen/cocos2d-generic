#ifndef COMMON_NODEFACTORY_H
#define COMMON_NODEFACTORY_H

#include "cocos2d.h"
#include <string>
#include <map>
#include <functional>
#include "json/document.h"

#define GET_NODE_FACTORY() common::coreModule::nodeFactory::getInstance()

namespace common {
	namespace coreModule {
		using namespace cocos2d;

		enum eNodeFactory : size_t {
			TRANSFORM_COMPONENT,
			SPRITE_COMPONENT,
			ANIMSPRITE_COMPONENT,
			LABEL_COMPONENT,
			BUTTON_COMPONENT,
			DRAGONBONES_COMPONENT,
			COLOR_COMPONENT,
			SCROLL_VIEW_COMPONENT,
		};

		static const std::vector<std::string> componentPriorityList = {
				{"buttonComponent"},
				{"spriteComponent"},
				{"labelComponent"},
				{"dragonbonesComponent"},
				{"transformComponent"},
				{"colorComponent"},
				{"scrollViewComponent"},
		};

		class nodeFactory {
		public:
			nodeFactory();
			~nodeFactory();
			static nodeFactory &getInstance();

			void getComponents(Node *node, const std::string &componentName,
									  const rapidjson::GenericValue<rapidjson::UTF8<char>>::Object &object);

			bool hasRegisteredComponent(const std::string &componentName);

			Node* createNodeWithType(const std::string &type);

			bool registerCustomNodeType(const std::string&, std::function<Node*()>);

		private:
			void init();
			bool inited = false;
		};
	}
}//common::coreModule


#endif //COMMON_NODEFACTORY_H
