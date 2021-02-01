#ifndef SWIPE_RPG_DATABASEMANAGER_H
#define SWIPE_RPG_DATABASEMANAGER_H

#include "cocos2d.h"
#include "common/databaseModule/databaseInterface.h"
#include <string>
#include <map>
#include <tuple>

#define GET_DATABASE_MANAGER() common::databaseModule::databaseManager::getInstance()

namespace common {
	namespace databaseModule {

		class databaseManager {
		public:
			databaseManager();
			~databaseManager();
			static databaseManager &getInstance();
			void registerDatabase(std::pair<std::string, std::string>, databaseInterface*);
			void executeLoadData();

			template <typename T>
			T* getDatabase(const std::string& key) {
				if (databasesMap.find(key) != databasesMap.end()) {
					return dynamic_cast<T*>(databasesMap[key]);
				}
				return nullptr;
			}

		private:
			std::map<std::string, databaseInterface*> databasesMap;
		};
	}
}


#endif //SWIPE_RPG_DATABASEMANAGER_H
