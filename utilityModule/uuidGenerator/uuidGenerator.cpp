#include "uuidGenerator.h"
#include "uuid.h"

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

std::string uuidGenerator::getRandom() {
    auto uuid = generator.getUUID();
    auto id = uuid.str();
    if (ids.count(id) != 0u)
        return getRandom();
    ids.insert(id);

    return id;
}

uuidGenerator::~uuidGenerator() {
    ids.clear();
}
