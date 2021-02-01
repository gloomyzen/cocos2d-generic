#include "databaseManager.h"

using namespace common;
using namespace common::databaseModule;

databaseManager *currentDBInstance = nullptr;

databaseManager::databaseManager() {
//	tileDatabaseInstance = new tilesDB("properties/database/tiles/db.json");
//	gameModesDatabaseInstance = new gameModesDB("properties/database/gameModes/db.json");
//	battleLevelsDatabaseInstance = new battleLevelsDB("properties/database/battleLevels/db.json");
//	questsDatabaseInstance = new questsDB("properties/database/quests/db.json");
}

databaseManager::~databaseManager() = default;

databaseManager &databaseManager::getInstance() {
	if (currentDBInstance == nullptr) {
		currentDBInstance = new databaseManager();
	}
	return *currentDBInstance;
}

void databaseManager::registerDatabase(std::pair<std::string, std::string> value, databaseInterface * db) {
	const auto& [key, path] = value;
	if (databasesMap.find(key) == databasesMap.end()) {
		db->setPath(path);
		databasesMap.insert({key, db});
	}
}

void databaseManager::executeLoadData() {
	for (const auto& [_, db] : databasesMap) {
		if (db->isLoaded())
			continue;
		db->executeLoadData();
	}
}
