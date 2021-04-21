#include "stringUtility.h"
#include <sstream>

std::string common::utilityModule::ltrim(std::string s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
	return s;
}

std::string common::utilityModule::rtrim(std::string s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(),
			s.end());
	return s;
}

std::string common::utilityModule::trim(std::string s) { return ltrim(rtrim(std::move(s))); }

std::wstring common::utilityModule::utf8_to_wstring(const std::string& str) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.from_bytes(str);
}

std::string common::utilityModule::wstring_to_utf8(const std::wstring& str) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.to_bytes(str);
}

std::string common::utilityModule::capitalizeString(std::string line) {
	for (auto it = line.begin(); it != line.end(); ++it) {
		if (it == line.begin()) {
			*it = std::toupper(*it);
		} else {
			*it = std::tolower(*it);
		}
	}
	return line;
}

std::string common::utilityModule::toLowerString(std::string line) {
	for (auto it = line.begin(); it != line.end(); ++it) { *it = std::tolower(*it); }
	return line;
}

std::vector<std::string> common::utilityModule::explodeString(const std::string& str, std::function<std::string(std::string)> clb) {
	std::vector<std::string> result;
	std::stringstream ssitem(str);
	while (ssitem.good()){
		std::string line;
		ssitem >> line;
		if (clb) {
			line = clb(line);
		}
		result.push_back(line);
	}

	return result;
}