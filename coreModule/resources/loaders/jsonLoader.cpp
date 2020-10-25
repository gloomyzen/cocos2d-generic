#include "jsonLoader.h"
#include "cocos2d.h"

using namespace mb::coreModule;


jsonLoader::jsonLoader() {}
jsonLoader::~jsonLoader() {}

rapidjson::Document jsonLoader::loadJson(const std::string &path) {
	auto jsonData = cocos2d::FileUtils::getInstance()->getStringFromFile(path + ".json");
	if (jsonData.empty()) {
		return nullptr;
	}
	rapidjson::Document document{};
	document.Parse<0>(jsonData.c_str());
	jsonData.clear();
	if (document.HasParseError()) {
		CCLOG("GetParseError %u\n", document.GetParseError());
	} else {
		return document;
	}
	return nullptr;
//#endif
}
