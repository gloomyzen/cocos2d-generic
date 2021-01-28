#include "cocos2d.h"
#include "common/debugModule/logManager.h"
#include "profileManager.h"
#include "common/coreModule/resources/resourceManager.h"
#include "common/coreModule/resources/loaders/jsonLoader.h"

using namespace common;
using namespace common::profileModule;

profileManager *profileMgrInstance = nullptr;

profileManager::profileManager() {}

profileManager::~profileManager() {
	auto deleteProfile = cocos2d::UserDefault::getInstance()->getBoolForKey("deleteProfile", false);
	if (!deleteProfile)
		save();
	profileBlocks.clear();
	profileBlocksClb.clear();
}

void profileManager::executeLoad() {
	load();
}

profileManager &profileManager::getInstance() {
	if (profileMgrInstance == nullptr) {
		profileMgrInstance = new profileManager();
	}
	return *profileMgrInstance;
}

void profileManager::load() {
	const std::string& path = "config/user_profile";
	auto defaultProfile = GET_JSON_MANAGER()->loadJson(path);
	auto profile = cocos2d::UserDefault::getInstance()->getStringForKey("profile", std::string());
	auto localProfile = GET_JSON_MANAGER()->stringToJson(profile);

	loadProfile(defaultProfile, localProfile);
}

void profileManager::save() {
	rapidjson::Document json;
	json.SetObject();
	rapidjson::Document::AllocatorType& allocator = json.GetAllocator();
	for (auto block : profileBlocks) {
		rapidjson::Value key(block.first.c_str(), allocator);
		rapidjson::Value value;
		if (block.second->save(value, allocator)) {
			json.AddMember(key, value, allocator);
		}
	}
	rapidjson::StringBuffer strbuf;
	strbuf.Clear();

	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	json.Accept(writer);

	std::cout << strbuf.GetString() << std::endl;
	cocos2d::UserDefault::getInstance()->setStringForKey("profile", strbuf.GetString());
}

void profileManager::loadProfile(const rapidjson::Document &defaultData, const rapidjson::Document &localData) {
	if (!defaultData.IsObject() || defaultData.HasParseError() || defaultData.IsNull()) {
		LOG_ERROR("profileManager::loadProfile: Object not found! Profile not loaded!");
		return;
	}
	auto localProfileValid = !localData.HasParseError() && !localData.IsNull() && localData.IsObject();

	for (auto it = defaultData.MemberBegin(); it != defaultData.MemberEnd(); ++it) {
		if (it->name.IsString() && it->value.IsObject()) {
			auto key = it->name.GetString();
			auto blockIt = profileBlocksClb.find(key);
			auto findBlock = blockIt != profileBlocksClb.end();

			if (!isBlockRegistered(key) && localProfileValid && findBlock) {
				auto localIt = localData.FindMember(key);
				if (localIt != localData.MemberEnd()) {
					auto block = blockIt->second();
					if (block->load(localIt->value.GetObjectJ())) {
						profileBlocks[key] = blockIt->second();
					}
					continue;
				}
			} else
			if (!isBlockRegistered(key)) {
				auto block = blockIt->second();
				if (block->load(it->value.GetObjectJ())) {
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
}

bool profileManager::isBlockRegistered(const std::string& needle) {
	return profileBlocks.count(needle) > 0;
}

profileBlockInterface *profileManager::getBlock(const std::string& key) {
	if (isBlockRegistered(key))
		return profileBlocks[key];
	return nullptr;
}

