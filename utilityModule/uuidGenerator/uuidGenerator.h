#pragma once
#ifndef GENERIC_UUIDGENERATOR_H
#define GENERIC_UUIDGENERATOR_H

#include <string>
#include <set>

#define GET_UUID_GENERATOR() generic::utilityModule::uuidGenerator::getInstance()

namespace generic::utilityModule {
    class uuidGenerator {
    public:
        ~uuidGenerator();
        static uuidGenerator& getInstance();
        static void cleanup();

        std::string getRandom(const unsigned int len = 5u);

    private:
        uuidGenerator() = default;
        uuidGenerator(const uuidGenerator&) = default;
        uuidGenerator& operator=(const uuidGenerator&) = default;
        static void create();
        static void onDeadReference();

        static uuidGenerator* pInstance;
        static bool destroyed;

        std::set<std::string> ids;
    };
}

#endif// GENERIC_UUIDGENERATOR_H
