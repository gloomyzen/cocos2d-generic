#ifndef COMMON_RESOURCEMANAGER_H
#define COMMON_RESOURCEMANAGER_H

#include <map>
#include <string>
#include "common/coreModule/resources/loaders/jsonLoader.h"

#define GET_RESOURCE_MANAGER() common::coreModule::resourceManager::getInstance()
#define GET_JSON_MANAGER() common::coreModule::resourceManager::getInstance().getJsonLoader()
#define GET_JSON(L) GET_JSON_MANAGER()->loadJson(L)
#define GET_JSON_PREF(L, K) GET_JSON_MANAGER()->loadJson(L, K)

namespace common {

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
}//common::coreModule


#endif //COMMON_RESOURCEMANAGER_H
