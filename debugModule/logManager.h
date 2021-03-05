#ifndef COMMON_LOGMANAGER_H
#define COMMON_LOGMANAGER_H

#include <iostream>
#include <string>
#include <vector>

#define GET_LOGGER() common::debugModule::loggerInstance::getInstance()
#define LOG_INFO(W) GET_LOGGER().info(W)
#define LOG_WARNING(W) GET_LOGGER().warning(W)
#define LOG_ERROR(W) GET_LOGGER().error(W)
#define LOGMANAGER_ITEMS_LIMIT 150

namespace common {
    namespace debugModule {

        enum class eLogTypes : size_t { LOG_INFO = 0, LOG_WARNING, LOG_ERROR };

        struct sLogMessage {
            eLogTypes type;
            std::string message;

            sLogMessage(eLogTypes _type, std::string _message) {
                type = _type;
                message = _message;
            }
        };

        /***
         * Данный класс нужен для логирования исключительных событий
         */
        class logManager {
          public:
            void info(const std::string& message);

            void warning(const std::string& message);

            void error(const std::string& message);

          private:
            void addLogMessage(sLogMessage*);

            std::vector<sLogMessage*> log{};
        };

        class loggerInstance {
          public:
            static logManager& getInstance();
        };
    }// namespace debugModule

}// namespace common

#endif// COMMON_LOGMANAGER_H