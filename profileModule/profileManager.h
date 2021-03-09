#ifndef COMMON_PROFILEMANAGER_H
#define COMMON_PROFILEMANAGER_H

#include "common/profileModule/profileBlockInterface.h"
#include <functional>
#include <map>
#include <string>

#define GET_PROFILE() common::profileModule::profileManager::getInstance()

namespace common {
    namespace profileModule {
        class locationProfile;

        class profileManager {
          public:
            profileManager();
            ~profileManager();
            static profileManager& getInstance();
            void executeLoad();

            bool registerBlock(const std::string&, const std::function<profileBlockInterface*()>&);
            bool isBlockRegistered(const std::string&);

            template<typename T>
            T* getBlock(const std::string& key) {
                if (isBlockRegistered(key))
                    return dynamic_cast<T*>(profileBlocks[key]);
                return nullptr;
            }

            void destroyProfile();
            void save();
            void cleanup();

          private:
            void load();
            void loadProfile(const rapidjson::Document& defaultData, const rapidjson::Document& localData);

            std::map<std::string, std::function<profileBlockInterface*()>> profileBlocksClb;
            std::map<std::string, profileBlockInterface*> profileBlocks;
        };
    }// namespace profileModule
}// namespace common


#endif// COMMON_PROFILEMANAGER_H
