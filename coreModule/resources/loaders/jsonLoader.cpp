#include "jsonLoader.h"
#include "cocos2d.h"
#include "generic/coreModule/resources/resourceManager.h"
#include "generic/coreModule/resources/settings/settingManager.h"
#include "generic/debugModule/logManager.h"
#include "generic/utilityModule/stringUtility.h"

using namespace generic::coreModule;

rapidjson::Document jsonLoader::loadJson(const std::string& path) {
    auto jsonStr = cocos2d::FileUtils::getInstance()->getStringFromFile(path + ".json");
    auto currentDeviceData = stringToJson(jsonStr);
    auto nextResolution = findByResolution(path);
    if (!nextResolution.IsNull()) {
        mergeJson(currentDeviceData, nextResolution, currentDeviceData.GetAllocator());
    }
    return currentDeviceData;
}

rapidjson::Document jsonLoader::stringToJson(const std::string& jsonStr) {
    if (jsonStr.empty()) {
        return nullptr;
    }
    rapidjson::Document document;
    document.Parse<0>(jsonStr.c_str());
    if (!document.HasParseError()) {
        return document;
    }
    LOG_ERROR(CSTRING_FORMAT("GetParseError %u\n", document.GetParseError()));
    return nullptr;
}

void jsonLoader::mergeJson(rapidjson::Value& target, rapidjson::Value& source, rapidjson::Value::AllocatorType& allocator) {
    if (!source.IsObject())
        return;

    for (auto item = source.MemberBegin(); item != source.MemberEnd(); ++item) {
        auto find = target.FindMember(item->name);
        if (find != target.MemberEnd() && find->value.IsObject()) {
            mergeJson(target[item->name], item->value, allocator);
        } else {
            target[item->name] = item->value;
        }
    }
}

rapidjson::Document jsonLoader::findByResolution(const std::string& path) {
    if (GET_RESOLUTION_SETTING().getCurrentSize() == nullptr || GET_RESOLUTION_SETTING().getCurrentSize()->getPath().empty()) {
        return nullptr;
    }
    auto resolutionPath = GET_RESOLUTION_SETTING().getCurrentSize()->getPath();
    auto pathProperties = STRING_FORMAT("%s/%s.json", resolutionPath.c_str(), path.c_str());

    if (!cocos2d::FileUtils::getInstance()->isFileExist(pathProperties)) {
        return nullptr;
    }
    auto jsonStr = cocos2d::FileUtils::getInstance()->getStringFromFile(pathProperties);

    return stringToJson(jsonStr);
}
