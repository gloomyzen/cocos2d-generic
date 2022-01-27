#include "uuidGenerator.h"
#include "uuid.h"
#include "cocos2d.h"

using namespace generic::utilityModule;

uuidGenerator* uuidGenerator::pInstance = nullptr;
bool uuidGenerator::destroyed = false;

uuidGenerator& uuidGenerator::getInstance() {
    if (!pInstance) {
        if (destroyed) {
            onDeadReference();
        } else {
            create();
        }
    }
    return *pInstance;
}

void uuidGenerator::cleanup() {
    destroyed = true;
    delete pInstance;
    pInstance = nullptr;
}

std::string uuidGenerator::getRandom(const unsigned int len) {
    auto id = generate_hex(len);
    if (ids.count(id) != 0u)
        return getRandom();
    ids.insert(id);

    return id;
}

uuidGenerator::~uuidGenerator() {
    ids.clear();
}

void uuidGenerator::create() {
    static uuidGenerator instance;
    pInstance = &instance;
}

void uuidGenerator::onDeadReference() {
    CCASSERT(false, "Founded dead reference!");
}
