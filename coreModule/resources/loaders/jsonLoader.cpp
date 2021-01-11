#include "jsonLoader.h"
#include "cocos2d.h"
#include "common/debugModule/logManager.h"

using namespace common::coreModule;


jsonLoader::jsonLoader() {}
jsonLoader::~jsonLoader() {}

rapidjson::Document jsonLoader::loadJson(const std::string &path) {
	auto jsonStr = cocos2d::FileUtils::getInstance()->getStringFromFile(path + ".json");

	return stringToJson(jsonStr);
}

rapidjson::Document jsonLoader::stringToJson(const std::string &jsonStr) {
	if (jsonStr.empty()) {
		return nullptr;
	}
	rapidjson::Document document{};
	document.Parse<0>(jsonStr.c_str());
	if (!document.HasParseError()) {
		return document;
	}
	LOG_ERROR(STRING_FORMAT("GetParseError %u\n", document.GetParseError()));
	return nullptr;
}
