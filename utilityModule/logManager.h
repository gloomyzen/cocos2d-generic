#pragma once
#ifndef GENERIC_LOGMANAGER_H
#define GENERIC_LOGMANAGER_H

#include "axmol.h"
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "generic/utilityModule/stringUtility.h"
#include <fmt/core.h>
#include <cassert>

#define __FMTLOG_S1(x) #x
#define __FMTLOG_S2(x) __FMTLOG_S1(x)
#define __FMTLOG_LOCATION __FILE__ ":" __FMTLOG_S2(__LINE__)

#define GET_LOGGER() generic::utilityModule::logManager::getInstance()
#define LOG_INFO(format, ...) GET_LOGGER().log(false, __FMTLOG_LOCATION, generic::utilityModule::eLogTypes::LOG_INFO, format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...) GET_LOGGER().log(false, __FMTLOG_LOCATION, generic::utilityModule::eLogTypes::LOG_WARNING, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) GET_LOGGER().log(false, __FMTLOG_LOCATION, generic::utilityModule::eLogTypes::LOG_ERROR, format, ##__VA_ARGS__)
#define LOG_ASSERT_ERROR(format, ...) GET_LOGGER().log(true, __FMTLOG_LOCATION, generic::utilityModule::eLogTypes::LOG_ERROR, format, ##__VA_ARGS__)
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

        template<typename... Args>
        inline void log(bool makeAssert, const char* location, eLogTypes level, std::string_view format, Args&&... args) {
            auto&& fmt_args = fmt::make_format_args(args...);
            auto fmt_str = fmt::vformat(format, fmt_args);
            auto fmt_full = fmt::format("{}: {}", location, fmt_str);
#ifdef DEBUG_ENABLED
            // using native axmol logging to the console
            switch (level) {
            case eLogTypes::LOG_ERROR:
                AXLOGERROR("[ERROR] %s", fmt_full.c_str());
                break;
            case eLogTypes::LOG_WARNING:
                AXLOGWARN("[WARNING] %s", fmt_full.c_str());
                break;
            case eLogTypes::LOG_INFO:
                AXLOG("[INFO] %s", fmt_full.c_str());
                break;
            }
#endif// DEBUG
            addLogMessage(sLogMessage(level, fmt_full));
            if (makeAssert) {
                assert(false && fmt_full.c_str());
            }
        }

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

        std::vector<sLogMessage> logVec;
    };
}// namespace generic::utilityModule

#endif// GENERIC_LOGMANAGER_H