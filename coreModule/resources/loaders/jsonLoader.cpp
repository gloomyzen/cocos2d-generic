#include "jsonLoader.h"
#include "cocos2d.h"
#include "common/debugModule/logManager.h"
#include "common/coreModule/resources/settings/settingManager.h"
#include "common/coreModule/resources/resourceManager.h"

using namespace common::coreModule;

jsonLoader::jsonLoader() {}
jsonLoader::~jsonLoader() {}

rapidjson::Document jsonLoader::loadJson(const std::string &path) {
	auto jsonStr = cocos2d::FileUtils::getInstance()->getStringFromFile(path + ".json");
	auto currentDeviceData = stringToJson(jsonStr);
	auto nextResolution = findByResolution(path);
	if (!nextResolution.IsNull()) {
		mergeJson(currentDeviceData, nextResolution, currentDeviceData.GetAllocator());
	}
	return currentDeviceData;
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

void jsonLoader::mergeJson(rapidjson::Value& target, rapidjson::Value& source, rapidjson::Value::AllocatorType& allocator) {
	if (!target.IsObject() || !source.IsObject()) return;

//	std::vector<rapidjson::GenericValue<rapidjson::UTF8<char>>::MemberIterator> list;
	for (auto item = source.MemberBegin(); item != source.MemberEnd(); ++item) {
		target.AddMember(item->name, item->value, allocator);
//		auto find = target.FindMember(item->name);
//		if (find != target.MemberEnd() && find->value.IsObject()) {
//			mergeJson(find->value, item->value, allocator);
//		} else {
//			list.push_back(item);
//		}
	}
//	for (const auto& item : list) {
//		target.AddMember(item->name, item->value, allocator);
//	}
}

rapidjson::Document jsonLoader::findByResolution(const std::string &path) {
	if (GET_RESOLUTION_SETTING()->getCurrentSize() == nullptr || GET_RESOLUTION_SETTING()->getCurrentSize()->getPath().empty()) {
		return nullptr;
	}
	std::string resolutionPath = GET_RESOLUTION_SETTING()->getCurrentSize()->getPath();
	std::string pathProperties = STRING_FORMAT("%s/%s.json", resolutionPath.c_str(), path.c_str());

	if (!cocos2d::FileUtils::getInstance()->isFileExist(pathProperties)) {
		return nullptr;
	}
	auto jsonStr = cocos2d::FileUtils::getInstance()->getStringFromFile(pathProperties);

	return stringToJson(jsonStr);
}
