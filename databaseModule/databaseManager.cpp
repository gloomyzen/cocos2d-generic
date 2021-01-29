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

void databaseManager::registerDatabase(std::string path, databaseInterface * db) {
	if (databasesMap.find(path) == databasesMap.end()) {
		databasesMap.insert({path, db});
	}
}

void databaseManager::executeLoadData() {
	for (const auto& [path, db] : databasesMap) {
		if (db->isLoaded())
			continue;
		db->setPath(path);
		db->executeLoadData();
	}
}
