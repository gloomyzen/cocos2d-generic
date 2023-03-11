#pragma once
#ifndef GENERIC_LOGMANAGER_H
#define GENERIC_LOGMANAGER_H

#include "axmol.h"
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "generic/utilityModule/stringUtility.h"

#define GET_LOGGER() generic::utilityModule::logManager::getInstance()
#define LOG_FORMAT(W) generic::utilityModule::stringToChar(W)
#define LOG_INFO(W) GET_LOGGER().info(ax::StringUtils::format("%s:[%d] : %s", __func__, __LINE__, LOG_FORMAT(W)))
#define LOG_WARNING(W) GET_LOGGER().warning(ax::StringUtils::format("%s:[%d] : %s", __FILE__, __LINE__, LOG_FORMAT(W)))
#define LOG_ERROR(W) GET_LOGGER().error(ax::StringUtils::format("%s:[%d] : %s", __FILE__, __LINE__, LOG_FORMAT(W)))
#define LOGMANAGER_ITEMS_LIMIT 150

namespace generic::utilityModule {

    enum class eLogTypes {
        LOG_INFO = 0,
        LOG_WARNING,
        LOG_ERROR
    };

    struct sLogMessage {
        eLogTypes type;
        std::string message;

        sLogMessage(eLogTypes _type, std::string _message) {
            type = _type;
            message = std::move(_message);
        }
    };

    /***
     * Logging events in application
     */
    class logManager {
    public:
        static logManager& getInstance();
        static void cleanup();

        void info(const std::string& message);
        void warning(const std::string& message);
        void error(const std::string& message);

    private:
        logManager() = default;
        ~logManager() = default;
        logManager(const logManager&) = default;
        logManager& operator=(const logManager&) = default;
        static void create();
        static void onDeadReference();

        static logManager* pInstance;
        static bool destroyed;

        void addLogMessage(const sLogMessage&);

        std::vector<sLogMessage> log;
    };
}// namespace generic::utilityModule

#endif// GENERIC_LOGMANAGER_H