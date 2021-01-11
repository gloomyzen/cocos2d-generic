#ifndef COMMON_RESOURCEMANAGER_H
#define COMMON_RESOURCEMANAGER_H

#include <map>
#include <string>
#include "common/coreModule/resources/loaders/jsonLoader.h"
#include "common/coreModule/resources/settings/settingManager.h"

#define GET_RESOURCE_MANAGER() common::coreModule::resourceManager::getInstance()
#define GET_JSON_MANAGER() common::coreModule::resourceManager::getInstance().getJsonLoader()
#define GET_RESOLUTION_SETTING() common::coreModule::resourceManager::getInstance().getSettingManager()
#define GET_JSON(L) GET_JSON_MANAGER()->loadJson(L)

namespace common {

	namespace coreModule {
		class resourceManager {
		public:
			resourceManager();
			~resourceManager();
			static resourceManager &getInstance();

			jsonLoader* getJsonLoader();
			settingManager* getSettingManager();

		private:
			jsonLoader jsonLoaderInstance;
			settingManager* settingManagerInstance = nullptr;
		};
	}
}//common::coreModule


#endif //COMMON_RESOURCEMANAGER_H
