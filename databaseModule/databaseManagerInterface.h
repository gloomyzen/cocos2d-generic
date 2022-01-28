#ifndef GENERIC_DATABASEMANAGER_H
#define GENERIC_DATABASEMANAGER_H

#include "cocos2d.h"
#include "generic/databaseModule/databaseInterface.h"
#include <map>
#include <string>
#include <tuple>
#include <memory>

namespace generic::databaseModule {

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

    protected:
        void registerDatabase(const std::pair<int, std::string>& value, const std::shared_ptr<databaseInterface>& db) {
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

        std::map<int, std::shared_ptr<databaseInterface>> databasesMap;
    };
}// namespace generic::databaseModule


#endif// GENERIC_DATABASEMANAGER_H
