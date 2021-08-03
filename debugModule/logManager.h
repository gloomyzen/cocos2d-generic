#ifndef GENERIC_LOGMANAGER_H
#define GENERIC_LOGMANAGER_H

#include "cocos2d.h"
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#define GET_LOGGER() generic::debugModule::loggerInstance::getInstance()
#define LOG_INFO(W) GET_LOGGER().info(cocos2d::StringUtils::format("m:%s -> %s:[%d] : %s", __func__, __FILE_NAME__, __LINE__, #W))
#define LOG_WARNING(W) GET_LOGGER().warning(cocos2d::StringUtils::format("m:%s -> %s:[%d] : %s", __func__, __FILE_NAME__, __LINE__, #W))

#define LOG_ERROR(W) GET_LOGGER().error(cocos2d::StringUtils::format("m:%s -> %s:[%d] : %s", __func__, __FILE_NAME__, __LINE__, #W))
#define LOGMANAGER_ITEMS_LIMIT 150

namespace generic::debugModule {

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
        void info(const std::string& message);

        void warning(const std::string& message);

        void error(const std::string& message);

    private:
        void addLogMessage(const sLogMessage&);

        std::vector<sLogMessage> log;
    };

    class loggerInstance {
    public:
        static logManager& getInstance();
        static void cleanup();
    };
}// namespace generic::debugModule

#endif// GENERIC_LOGMANAGER_H