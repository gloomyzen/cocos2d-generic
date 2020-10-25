#ifndef MERCENARY_BATTLES_RESOURCEMANAGER_H
#define MERCENARY_BATTLES_RESOURCEMANAGER_H

#include <map>
#include <string>
#include "coreModule/resources/loaders/jsonLoader.h"

#define GET_RESOURCE_MANAGER() mb::coreModule::resourceManager::getInstance()
#define GET_JSON_MANAGER() mb::coreModule::resourceManager::getInstance().getJsonLoader()
#define GET_JSON(L) GET_JSON_MANAGER()->loadJson(L)
#define GET_JSON_PREF(L, K) GET_JSON_MANAGER()->loadJson(L, K)

namespace mb {

	namespace coreModule {
		class resourceManager {
		public:
			resourceManager();
			~resourceManager();
			static resourceManager &getInstance();

			jsonLoader* getJsonLoader();

		private:
			jsonLoader jsonLoaderInstance;
		};
	}
}//mb::coreModule


#endif //MERCENARY_BATTLES_RESOURCEMANAGER_H
