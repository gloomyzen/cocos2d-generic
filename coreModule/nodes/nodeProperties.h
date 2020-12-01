#ifndef COMMON_NODEPROPERTIES_H
#define COMMON_NODEPROPERTIES_H

#include "common/debugModule/logManager.h"
#include "cocos2d.h"
#include <string>
#include "json/document.h"

namespace common {
	using namespace cocos2d;

	namespace coreModule {

		class nodeProperties {
		public:
			/***
		 	 * Парсинг параметров из json файла
		 	 * @param path relative path to file
			 * @param node Node instance
		 	 */
			void loadProperty(const std::string &path, Node *node);
			void parseComponents(Node *node, const std::string &string, bool recursive = false);

		private:
			void parseData(Node *node, const rapidjson::GenericValue<rapidjson::UTF8<char>>::Array &array);
		};
	}
}//common::coreModule

#endif //COMMON_NODEPROPERTIES_H
