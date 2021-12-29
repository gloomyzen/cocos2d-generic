#ifndef GENERIC_RESOURCEMANAGER_H
#define GENERIC_RESOURCEMANAGER_H

#include "generic/coreModule/resources/loaders/jsonLoader.h"
#include "generic/coreModule/resources/settings/settingManager.h"
#include <map>
#include <vector>
#include <string>

#define GET_RESOURCE_MANAGER() generic::coreModule::resourceManager::getInstance()
#define GET_JSON_MANAGER() generic::coreModule::resourceManager::getInstance().getJsonLoader()
#define GET_RESOLUTION_SETTING() generic::coreModule::resourceManager::getInstance().getSettingManager()
#define GET_JSON(L) GET_JSON_MANAGER()->loadJson(L)

namespace generic {

    namespace coreModule {
        class resourceManager {
        public:
            resourceManager();
            ~resourceManager();
            static resourceManager& getInstance();

            jsonLoader* getJsonLoader();
            settingManager* getSettingManager();
            std::string getImagePathWithExtension(const std::string& path);

        private:
            jsonLoader jsonLoaderInstance;
            settingManager* settingManagerInstance = nullptr;
            static std::vector<std::string> imageExtensions;
        };
    }// namespace coreModule
}// namespace generic


#endif// GENERIC_RESOURCEMANAGER_H
