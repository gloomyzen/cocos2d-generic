#ifndef GENERIC_UUIDGENERATOR_H
#define GENERIC_UUIDGENERATOR_H

#include "uuid.h"
#include <string>
#include <set>

#define GET_UUID_GENERATOR() generic::utilityModule::uuidGenerator::getInstance()

namespace generic::utilityModule {
    class uuidGenerator {
    public:
        ~uuidGenerator();
        static uuidGenerator& getInstance();
        void cleanup();

        std::string getRandom();

    private:
        UUIDv4::UUIDGenerator<std::mt19937_64> generator;
        std::set<std::string> ids;
    };
}

#endif// GENERIC_UUIDGENERATOR_H
