#include "settingManager.h"
#include "json/document.h"
#include "json/ostreamwrapper.h"
#include "common/debugModule/logManager.h"

using namespace common::coreModule;

settingManager::settingManager() {

}

void settingManager::load() {
	auto defaultResolution = new sDisplaySize(1024.f, 768.f, 1.f);

	// load json
	const std::string &regionStr = cocos2d::FileUtils::getInstance()->getStringFromFile("config/setting.json");
	rapidjson::Document doc;
	doc.Parse<0>(regionStr.c_str());

	if (doc.HasParseError()) {
		LOG_ERROR("settingManager::load: json parse error");
		allResolutions.push_back(defaultResolution);
		return;
	}

	if (doc.IsNull()) {
		LOG_ERROR("settingManager::load: json is empty");
		allResolutions.push_back(defaultResolution);
		return;
	}

	for (auto it = doc.GetObjectJ().begin(); it != doc.GetObjectJ().end(); ++it) {
		auto test = it->name.GetString();
		auto test2 = it->value.GetObjectJ();
	}
}
