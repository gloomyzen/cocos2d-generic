#include "resourceManager.h"
#include "cocos2d.h"
#include "generic/debugModule/logManager.h"

using namespace generic::coreModule;

std::vector<std::string> resourceManager::imageExtensions = { "png", "jpeg", "jpg" };

resourceManager* currentResourceManager = nullptr;

resourceManager::resourceManager() {
    settingManagerInstance = new settingManager();
}

resourceManager::~resourceManager() {
    delete settingManagerInstance;
    settingManagerInstance = nullptr;
}

resourceManager& resourceManager::getInstance() {
    if (currentResourceManager == nullptr) {
        currentResourceManager = new resourceManager();
        // todo update for ios and ipad
        //		auto fileUtils = cocos2d::FileUtils::getInstance();
        //		std::vector<std::string> searchPaths = fileUtils->getSearchPaths();
        //		searchPaths.insert(searchPaths.begin(), "iphone");
        //		searchPaths.insert(searchPaths.begin(), "ipad");
        //		fileUtils->setSearchPaths(searchPaths);
    }
    return *currentResourceManager;
}

jsonLoader* resourceManager::getJsonLoader() {
    return &jsonLoaderInstance;
}

settingManager* resourceManager::getSettingManager() {
    return settingManagerInstance;
}
std::string resourceManager::getImagePathWithExtension(const std::string& path) {
    for (const auto& extension : imageExtensions) {
        auto fullPath = cocos2d::StringUtils::format("%s.%s", path.c_str(), extension.c_str());
        if (cocos2d::FileUtils::getInstance()->isFileExist(fullPath)) {
            return fullPath;
        }
    }
    return std::string();
}
