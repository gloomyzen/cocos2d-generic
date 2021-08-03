#include "profileManager.h"
#include "cocos2d.h"
#include "generic/coreModule/resources/loaders/jsonLoader.h"
#include "generic/coreModule/resources/resourceManager.h"
#include "generic/debugModule/logManager.h"
#include "generic/utilityModule/jsonHelper.h"
#include "generic/utilityModule/stringUtility.h"

using namespace generic;
using namespace generic::profileModule;

#ifdef CMAKE_APP_NAME
const std::string APP_NAME = CMAKE_APP_NAME;
#else
const std::string APP_NAME = "defaultAppName";
#endif

profileManager* profileMgrInstance = nullptr;

profileManager::profileManager() {}

profileManager::~profileManager() {
    cleanup();
}

void profileManager::executeLoad() {
    load();
}

profileManager& profileManager::getInstance() {
    if (profileMgrInstance == nullptr) {
        profileMgrInstance = new profileManager();
    }
    return *profileMgrInstance;
}

void profileManager::load() {
    const std::string& path = "config/user_profile";
    auto defaultProfile = GET_JSON_MANAGER()->loadJson(path);
    auto profile =
      cocos2d::UserDefault::getInstance()->getStringForKey(STRING_FORMAT("profile_%s", APP_NAME.c_str()).c_str(), std::string());
    auto localProfile = GET_JSON_MANAGER()->stringToJson(profile);
#ifdef DEBUG
    if (!localProfile.HasParseError() && !localProfile.IsNull()) {
        rapidjson::StringBuffer strBuf;
        strBuf.Clear();

        rapidjson::Writer<rapidjson::StringBuffer> writer(strBuf);
        localProfile.Accept(writer);
        std::cout << STRING_FORMAT("local profile: %s", strBuf.GetString()) << std::endl;
    }
#endif
    loadProfile(defaultProfile, localProfile);
}

void profileManager::save() {
    rapidjson::Document json;
    json.SetObject();
    rapidjson::Document::AllocatorType& allocator = json.GetAllocator();
    for (auto block : profileBlocks) {
        rapidjson::Value key(block.first.c_str(), allocator);
        rapidjson::Value value;
        value.SetObject();
        if (block.second->save(value, allocator)) {
            json.AddMember(key, value, allocator);
        }
    }
    rapidjson::StringBuffer strBuf;
    strBuf.Clear();

    rapidjson::Writer<rapidjson::StringBuffer> writer(strBuf);
    json.Accept(writer);
#ifdef DEBUG
    std::cout << STRING_FORMAT("save local profile: %s", strBuf.GetString()) << std::endl;
#endif

    cocos2d::UserDefault::getInstance()->setStringForKey(STRING_FORMAT("profile_%s", APP_NAME.c_str()).c_str(), strBuf.GetString());
}

void profileManager::loadProfile(const rapidjson::Document& defaultData, const rapidjson::Document& localData) {
    if (!defaultData.IsObject() || defaultData.HasParseError() || defaultData.IsNull()) {
        LOG_ERROR("Object not found! Profile not loaded!");
        return;
    }
    auto localProfileValid = !localData.HasParseError() && !localData.IsNull() && localData.IsObject();

    for (auto it = defaultData.MemberBegin(); it != defaultData.MemberEnd(); ++it) {
        if (it->name.IsString() && it->value.IsObject()) {
            auto key = it->name.GetString();
            auto blockIt = profileBlocksClb.find(key);
            if (blockIt == profileBlocksClb.end())
                continue;

            if (!isBlockRegistered(key) && localProfileValid) {
                auto localIt = localData.FindMember(key);
                if (localIt != localData.MemberEnd() && localIt->value.IsObject()) {
                    auto block = blockIt->second();
                    if (block->load(localIt->value.GetObject())) {
                        profileBlocks[key] = block;
                        continue;
                    }
                }
            }
            if (!isBlockRegistered(key)) {
                auto block = blockIt->second();
                if (block->load(it->value.GetObject())) {
                    profileBlocks[key] = block;
                }
            }
        }
    }
}

bool profileManager::registerBlock(const std::string& key, const std::function<profileBlockInterface*()>& blockClb) {
    if (isBlockRegistered(key))
        return false;

    profileBlocksClb[key] = blockClb;
    return true;
}

bool profileManager::isBlockRegistered(const std::string& needle) {
    return profileBlocks.count(needle) > 0;
}

void profileManager::destroyProfile() {
    cocos2d::UserDefault::getInstance()->deleteValueForKey("profile");
}

void profileManager::cleanup() {
    auto deleteProfile = cocos2d::UserDefault::getInstance()->getBoolForKey("deleteProfile", false);
    if (!deleteProfile)
        save();
    for (auto& item : profileBlocks) {
        delete item.second;
        item.second = nullptr;
    }
    for (auto& item : profileBlocksClb) {
        item.second = nullptr;
    }
    profileBlocks.clear();
    profileBlocksClb.clear();
}
