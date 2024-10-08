#include "resourceManager.h"
#include "axmol.h"
#include "generic/utilityModule/logManager.h"

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
    jsonLoaderInstance = nullptr;
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
    pInstance = nullptr;
}

void resourceManager::create() {
    static resourceManager instance;
    pInstance = &instance;
}

void resourceManager::onDeadReference() {
    AXASSERT(false, "Founded dead reference!");
}

jsonLoader* resourceManager::getJsonLoader() {
    return jsonLoaderInstance;
}

settingManager* resourceManager::getSettingManager() {
    return settingManagerInstance;
}

std::string resourceManager::getImagePathWithExtension(const std::string& path) {
    for (const auto& extension : imageExtensions) {
        auto fullPath = ax::StringUtils::format("%s.%s", path.c_str(), extension.c_str());
        if (ax::FileUtils::getInstance()->isFileExist(fullPath)) {
            return fullPath;
        }
    }
    return std::string();
}
