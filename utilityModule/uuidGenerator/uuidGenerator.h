#ifndef GENERIC_UUIDGENERATOR_H
#define GENERIC_UUIDGENERATOR_H

#include "uuid.h"
#include <string>
#include <map>

#define GET_UUID_GENERATOR() generic::utilityModule::uuidGenerator::getInstance()

namespace generic::utilityModule {
    class uuidGenerator {
    public:
        static uuidGenerator& getInstance();
        static void cleanup();
    };
}

#endif// GENERIC_UUIDGENERATOR_H
