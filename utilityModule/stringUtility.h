#ifndef COMMON_STRINGUTILITY_H
#define COMMON_STRINGUTILITY_H

#include <algorithm>
#include <cctype>
#include <string>
#include <clocale>
#include <locale>
#include <codecvt>
#include <utility>

namespace common::utilityModule {
	class stringUtility {
	public:
		static inline std::string ltrim(std::string s) {
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
				return !std::isspace(ch);
			}));
			return s;
		}

		static inline std::string rtrim(std::string s) {
			s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
				return !std::isspace(ch);
			}).base(), s.end());
			return s;
		}

		static inline std::string trim(std::string s) {
			return ltrim(rtrim(std::move(s)));
		}

		static std::wstring utf8_to_wstring(const std::string &str) {
			std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
			return conv.from_bytes(str);
		}

		static std::string wstring_to_utf8(const std::wstring &str) {
			std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
			return conv.to_bytes(str);
		}

		static std::string capitalizeString(std::string line) {
			for (auto it = line.begin(); it != line.end(); ++it) {
				if (it == line.begin()) {
					*it = std::toupper(*it);
				} else {
					*it = std::tolower(*it);
				}
			}
			return line;
		};

		static std::string toLowerString(std::string line) {
			for (auto it = line.begin(); it != line.end(); ++it) {
				*it = std::tolower(*it);
			}
			return line;
		};
	};
}

#endif //COMMON_STRINGUTILITY_H
