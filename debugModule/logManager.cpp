#include "logManager.h"
#include "cocos2d.h"
#include <map>

using namespace generic::debugModule;

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

logManager* mLoggerInstance = nullptr;

logManager& loggerInstance::getInstance() {
    if (mLoggerInstance == nullptr) {
        mLoggerInstance = new logManager();
    }
    return *mLoggerInstance;
}

void loggerInstance::cleanup() {
    delete mLoggerInstance;
    mLoggerInstance = nullptr;
}
