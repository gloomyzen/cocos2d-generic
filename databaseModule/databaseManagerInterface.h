#ifndef COMMON_DATABASEMANAGER_H
#define COMMON_DATABASEMANAGER_H

#include "cocos2d.h"
#include "common/databaseModule/databaseInterface.h"
#include <map>
#include <string>
#include <tuple>

namespace common::databaseModule {

    class databaseManagerInterface {
      public:
        virtual ~databaseManagerInterface() = default;

        void executeLoadData() {
            for (const auto& [_, db] : databasesMap) {
                if (db->isLoaded())
                    continue;
                db->executeLoadData();
            }
        }

        virtual void cleanup() = 0;

      protected:
        void registerDatabase(const std::pair<int, std::string>& value, databaseInterface* db) {
            const auto& [key, path] = value;
            if (databasesMap.find(key) == databasesMap.end()) {
                db->setPath(path);
                databasesMap.insert({ key, db });
            }
        }

        template<typename T>
        T* getRegisteredDatabase(int key) {
            if (databasesMap.find(key) != databasesMap.end()) {
                return dynamic_cast<T*>(databasesMap[key]);
            }
            return nullptr;
        }

        std::map<int, databaseInterface*> databasesMap;
    };
}// namespace common::databaseModule


#endif// COMMON_DATABASEMANAGER_H
