#include "resourceManager.h"
#include "common/debugModule/logManager.h"
#include "cocos2d.h"

using namespace common::coreModule;

resourceManager *currentResourceManager = nullptr;

resourceManager::resourceManager() {}

resourceManager::~resourceManager() {}

resourceManager &resourceManager::getInstance() {
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

jsonLoader *resourceManager::getJsonLoader() {
	return &jsonLoaderInstance;
}
