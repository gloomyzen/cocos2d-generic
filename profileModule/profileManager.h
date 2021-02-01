#ifndef SWIPE_RPG_PROFILEMANAGER_H
#define SWIPE_RPG_PROFILEMANAGER_H

#include "common/profileModule/profileBlockInterface.h"
#include <map>
#include <string>
#include <functional>

#define GET_PROFILE() common::profileModule::profileManager::getInstance()

namespace common {
	namespace profileModule {
		class locationProfile;

		class profileManager {
		public:
			profileManager();
			~profileManager();
			static profileManager &getInstance();
			void executeLoad();

			bool registerBlock(const std::string&, const std::function<profileBlockInterface*()>&);
			bool isBlockRegistered(const std::string&);

			template <typename T>
			T* getBlock(const std::string& key) {
				if (isBlockRegistered(key))
					return dynamic_cast<T*>(profileBlocks[key]);
				return nullptr;
			}

			void destroyProfile();
			void save();

		private:
			void load();
			void loadProfile(const rapidjson::Document& defaultData, const rapidjson::Document& localData);

			std::map<std::string, std::function<profileBlockInterface*()>> profileBlocksClb;
			std::map<std::string, profileBlockInterface*> profileBlocks;
		};
	}
}


#endif //SWIPE_RPG_PROFILEMANAGER_H
