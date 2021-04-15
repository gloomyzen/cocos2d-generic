#ifndef COMMON_STRINGUTILITY_H
#define COMMON_STRINGUTILITY_H

#include <algorithm>
#include <cctype>
#include <clocale>
#include <codecvt>
#include <locale>
#include <string>
#include <utility>
#include <vector>
#include <functional>

#define STRING_FORMAT(string, ...) cocos2d::StringUtils::format(string, ##__VA_ARGS__)

namespace common::utilityModule {

	std::string ltrim(std::string s);

	std::string rtrim(std::string s);

	std::string trim(std::string s);

	std::wstring utf8_to_wstring(const std::string& str);

	std::string wstring_to_utf8(const std::wstring& str);

	std::string capitalizeString(std::string line);

	std::string toLowerString(std::string line);

	std::vector<std::string> explodeString(const std::string& str, std::function<std::string(std::string)> clb = nullptr);

}// namespace common::utilityModule

#endif// COMMON_STRINGUTILITY_H
