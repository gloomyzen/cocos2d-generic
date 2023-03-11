#pragma once
#ifndef GENERIC_STRINGUTILITY_H
#define GENERIC_STRINGUTILITY_H

#include "axmol.h"
#include <algorithm>
#include <cctype>
#include <clocale>
#include <codecvt>
#include <functional>
#include <locale>
#include <string>
#include <utility>
#include <vector>

#define STRING_FORMAT(string, ...) ax::StringUtils::format(string, ##__VA_ARGS__)
#define CSTRING_FORMAT(string, ...) generic::utilityModule::stringToChar(STRING_FORMAT(string, ##__VA_ARGS__))


namespace generic::utilityModule {

    std::string ltrim(std::string s);

    std::string rtrim(std::string s);

    std::string trim(std::string s);

    std::wstring utf8_to_wstring(const std::string& str);

    std::string wstring_to_utf8(const std::wstring& str);

    std::string capitalizeString(std::string line);

    std::string toLowerString(std::string line);

    std::vector<std::string> explodeString(const std::string& str, const std::function<std::string(std::string)>& clb = nullptr);

    /*
     * Helper for macros
     */
    const char* stringToChar(const std::string& str);
    const char* stringToChar(const char* str);

}// namespace generic::utilityModule

#endif// GENERIC_STRINGUTILITY_H
