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
		mergeJson(document, document, document.GetAllocator());
		return document;
	}
	LOG_ERROR(STRING_FORMAT("GetParseError %u\n", document.GetParseError()));
	return nullptr;
}

void jsonLoader::mergeJson(rapidjson::Value& target, rapidjson::Value& source, rapidjson::Value::AllocatorType& allocator) {
	if (!target.IsObject() || !source.IsObject()) return;

	std::vector<rapidjson::GenericValue<rapidjson::UTF8<char>>::MemberIterator> list;
	for (rapidjson::Value::MemberIterator item = source.MemberBegin(); item != source.MemberEnd(); ++item) {
		auto find = target.FindMember(item->name);
		if (find != target.MemberEnd() && find->value.IsObject()) {
			mergeJson(find->value, item->value, allocator);
		} else {
			list.push_back(item);
		}
	}
	for (const auto& item : list) {
		target.AddMember(item->name, item->value, allocator);
	}
}
