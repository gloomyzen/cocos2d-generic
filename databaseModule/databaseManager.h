#ifndef SWIPE_RPG_DATABASEMANAGER_H
#define SWIPE_RPG_DATABASEMANAGER_H

#include "cocos2d.h"
#include "common/databaseModule/databaseInterface.h"
#include <string>
#include <map>

#define GET_DATABASE_MANAGER() common::databaseModule::databaseManager::getInstance()

namespace common {
	namespace databaseModule {

		class databaseManager {
		public:
			databaseManager();
			~databaseManager();
			static databaseManager &getInstance();
			void registerDatabase(std::string, databaseInterface*);
			void executeLoadData();

		private:
			std::map<std::string, databaseInterface*> databasesMap;
		};
	}
}


#endif //SWIPE_RPG_DATABASEMANAGER_H
