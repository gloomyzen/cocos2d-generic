#ifndef GENERIC_UUID_H
#define GENERIC_UUID_H

#include <sstream>
#include <random>
#include <string>

namespace generic::utilityModule {

    unsigned int random_char() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        return dis(gen);
    }

    std::string generate_hex(const unsigned int len) {
        std::stringstream ss;
        for (auto i = 0; i < len; i++) {
            const auto rc = random_char();
            std::stringstream hexStream;
            hexStream << std::hex << rc;
            auto hex = hexStream.str();
            ss << (hex.length() < 2 ? '0' + hex : hex);
        }
        return ss.str();
    }

}

#endif// GENERIC_UUID_H
