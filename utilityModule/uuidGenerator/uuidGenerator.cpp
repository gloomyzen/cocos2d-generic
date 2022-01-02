#include "uuidGenerator.h"

using namespace generic::utilityModule;

uuidGenerator* currentUuidGenerator = nullptr;

uuidGenerator& uuidGenerator::getInstance() {
    if (currentUuidGenerator == nullptr) {
        currentUuidGenerator = new uuidGenerator();
    }
    return *currentUuidGenerator;
}

void uuidGenerator::cleanup() {
    delete currentUuidGenerator;
    currentUuidGenerator = nullptr;
}
