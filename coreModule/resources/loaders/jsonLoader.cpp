#include "jsonLoader.h"
#include "axmol.h"
#include "generic/coreModule/resources/resourceManager.h"
#include "generic/coreModule/resources/settings/settingManager.h"
#include "generic/utilityModule/logManager.h"
#include "generic/utilityModule/stringUtility.h"
#include "magic_enum.hpp"
#include <fmt/core.h>

using namespace generic::coreModule;

rapidjson::Document jsonLoader::loadJson(const std::string& path) {
    auto jsonStr = ax::FileUtils::getInstance()->getStringFromFile(path + ".json");
    auto currentDeviceData = stringToJson(jsonStr);
    auto nextResolution = findByResolution(path);
    if (!nextResolution.IsNull()) {
        rapidjson::StringBuffer strBuf;
        strBuf.Clear();

        rapidjson::Writer<rapidjson::StringBuffer> writer(strBuf);
        currentDeviceData.Accept(writer);
        LOG_INFO("before current: {}", strBuf.GetString());
        strBuf.Clear();

        rapidjson::Writer<rapidjson::StringBuffer> writer2(strBuf);
        currentDeviceData.Accept(writer2);
        LOG_INFO("next current: {}", strBuf.GetString());
        mergeObjects(currentDeviceData, nextResolution, currentDeviceData.GetAllocator());
        strBuf.Clear();

        rapidjson::Writer<rapidjson::StringBuffer> writer3(strBuf);
        currentDeviceData.Accept(writer3);
        LOG_INFO("final current: {}", strBuf.GetString());
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
    auto errorType = magic_enum::enum_name(document.GetParseError());
    LOG_ERROR("GetParseError {}\n", errorType);
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
            target.AddMember(item->name, item->value, allocator);
        }
    }
}

rapidjson::Document jsonLoader::findByResolution(const std::string& path) {
    if (GET_RESOLUTION_SETTING()->getCurrentSize() == nullptr || GET_RESOLUTION_SETTING()->getCurrentSize()->getPath().empty()) {
        return nullptr;
    }
    auto resolutionPath = GET_RESOLUTION_SETTING()->getCurrentSize()->getPath();
    auto pathProperties = fmt::format("{}/{}.json", resolutionPath.c_str(), path.c_str());

    if (!ax::FileUtils::getInstance()->isFileExist(pathProperties)) {
        return nullptr;
    }
    auto jsonStr = ax::FileUtils::getInstance()->getStringFromFile(pathProperties);

    return stringToJson(jsonStr);
}

bool jsonLoader::mergeObjects(rapidjson::Value& dist, rapidjson::Value& src, rapidjson::Document::AllocatorType& allocator) {
    for (auto srcIt = src.MemberBegin(); srcIt != src.MemberEnd(); ++srcIt) {
        auto dstIt = dist.FindMember(srcIt->name);
        if (dstIt == dist.MemberEnd()) {
            rapidjson::Value dstName;
            dstName.CopyFrom(srcIt->name, allocator);
            rapidjson::Value dstVal;
            dstVal.CopyFrom(srcIt->value, allocator);

            dist.AddMember(dstName, dstVal, allocator);

            dstName.CopyFrom(srcIt->name, allocator);
            dstIt = dist.FindMember(dstName);
            if (dstIt == dist.MemberEnd())
                return false;
        } else {
            auto srcT = srcIt->value.GetType();
            auto dstT = dstIt->value.GetType();
            if (srcT != dstT)
                return false;

            if (srcIt->value.IsArray()) {
                dstIt->value.CopyFrom(srcIt->value, allocator);
            } else if (srcIt->value.IsObject()) {
                if (!mergeObjects(dstIt->value, srcIt->value, allocator))
                    return false;
            } else {
                dstIt->value.CopyFrom(srcIt->value, allocator);
            }
        }
    }

    return true;
}