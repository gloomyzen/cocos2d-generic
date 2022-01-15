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
        void cleanup();

        std::string getRandom(const unsigned int len = 5u);

    private:
        std::set<std::string> ids;
    };
}

#endif// GENERIC_UUIDGENERATOR_H
