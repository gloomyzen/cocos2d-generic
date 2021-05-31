#include "signalInstance.h"

using namespace common::coreModule;

signalInstance* currentSignalInstance = nullptr;

signalInstance::signalInstance() {

}

signalInstance::~signalInstance() {
    //
}

signalInstance& signalInstance::getInstance() {
    if (currentSignalInstance == nullptr) {
        currentSignalInstance = new signalInstance();
    }
    return *currentSignalInstance;
}

void signalInstance::clean() {
    delete currentSignalInstance;
    currentSignalInstance = nullptr;
}
