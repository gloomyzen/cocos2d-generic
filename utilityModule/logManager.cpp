#include "logManager.h"
#include "axmol.h"
#include <map>

using namespace generic::utilityModule;

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
    AXASSERT(false, "Founded dead reference!");
}

void logManager::addLogMessage(const sLogMessage& message) {
    logVec.emplace_back(message);
    if (logVec.size() > LOGMANAGER_ITEMS_LIMIT) {
        logVec.erase(logVec.begin());
    }
}
