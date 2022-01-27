#include "resourceManager.h"
#include "cocos2d.h"
#include "generic/debugModule/logManager.h"

using namespace generic::coreModule;

std::vector<std::string> resourceManager::imageExtensions = { "png", "jpeg", "jpg" };
resourceManager* resourceManager::pInstance = nullptr;
bool resourceManager::destroyed = false;
jsonLoader* resourceManager::jsonLoaderInstance = nullptr;
settingManager* resourceManager::settingManagerInstance = nullptr;

resourceManager::resourceManager() {
    static jsonLoader jsonInstance;
    jsonLoaderInstance = &jsonInstance;
    static settingManager settingMgrInstance;
    settingManagerInstance = &settingMgrInstance;
}

resourceManager::~resourceManager() {
    delete jsonLoaderInstance;
    jsonLoaderInstance = nullptr;
    delete settingManagerInstance;
    settingManagerInstance = nullptr;
}

resourceManager& resourceManager::getInstance() {
    if (!pInstance) {
        if (destroyed) {
            onDeadReference();
        } else {
            create();
        }
    }
    return *pInstance;
}

void resourceManager::cleanup() {
    destroyed = true;
    delete pInstance;
    pInstance = nullptr;
}

void resourceManager::create() {
    static resourceManager instance;
    pInstance = &instance;
}

void resourceManager::onDeadReference() {
    CCASSERT(false, "Founded dead reference!");
}

jsonLoader* resourceManager::getJsonLoader() {
    return jsonLoaderInstance;
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
