#ifndef GENERIC_PROFILEMANAGER_H
#define GENERIC_PROFILEMANAGER_H

#include "profileBlockInterface.h"
#include <functional>
#include <map>
#include <string>

#define GET_PROFILE() generic::profileModule::profileManager::getInstance()
#define SAVE_PROFILE() generic::profileModule::profileManager::getInstance().save();


namespace generic::profileModule {

    class profileManager {
    public:
        ~profileManager();
        static profileManager& getInstance();
        static void cleanup();
        void executeLoad();

        bool registerBlock(const std::string&, const std::function<profileBlockInterface*()>&);
        bool isBlockRegistered(const std::string&);

        template<typename T>
        T* getBlock(const std::string& key) {
            if (isBlockRegistered(key))
                return dynamic_cast<T*>(profileBlocks[key]);
            return nullptr;
        }

        static void destroyProfile();
        void save();

    private:
        profileManager() = default;
        profileManager(const profileManager&) = default;
        profileManager& operator=(const profileManager&) = default;
        static void create();
        static void onDeadReference();

        static profileManager* pInstance;
        static bool destroyed;

        void load();
        void loadProfile(const rapidjson::Document& defaultData, const rapidjson::Document& localData);

        std::map<std::string, std::function<profileBlockInterface*()>> profileBlocksClb;
        std::map<std::string, profileBlockInterface*> profileBlocks;
    };
}// namespace generic::profileModule


#endif// GENERIC_PROFILEMANAGER_H
