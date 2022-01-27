#include "logManager.h"
#include "cocos2d.h"
#include <map>

using namespace generic::debugModule;

logManager* logManager::pInstance = nullptr;
bool logManager::destroyed = false;

logManager& logManager::getInstance() {
    if (!pInstance) {
        if (destroyed) {
            onDeadReference();
        } else {
            create();
        }
    }
    return *pInstance;
}

void logManager::cleanup() {
    destroyed = true;
    pInstance = nullptr;
}

void logManager::create() {
    static logManager instance;
    pInstance = &instance;
}

void logManager::onDeadReference() {
    CCASSERT(false, "Founded dead reference!");
}

void logManager::info(const std::string& message) {
#ifdef DEBUG
    CCLOG("[INFO] %s", message.c_str());
#endif// DEBUG
    addLogMessage(sLogMessage(eLogTypes::LOG_INFO, message));
}

void logManager::warning(const std::string& message) {
#ifdef DEBUG
    CCLOGWARN("[WARNING] %s", message.c_str());
#endif// DEBUG
    addLogMessage(sLogMessage(eLogTypes::LOG_WARNING, message));
}

void logManager::error(const std::string& message) {
#ifdef DEBUG
    CCLOGWARN("[ERROR] %s", message.c_str());
#endif// DEBUG
    addLogMessage(sLogMessage(eLogTypes::LOG_ERROR, message));
}

void logManager::addLogMessage(const sLogMessage& message) {
    log.emplace_back(message);
    if (log.size() > LOGMANAGER_ITEMS_LIMIT) {
        log.erase(log.begin());
    }
}
