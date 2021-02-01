#ifndef COMMON_STRINGUTILITY_H
#define COMMON_STRINGUTILITY_H

#include <algorithm>
#include <cctype>
#include <locale>
#include <string>

namespace common {
	namespace utilityModule {

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
				return ltrim(rtrim(s));
			}
		};
	}
}

#endif //COMMON_STRINGUTILITY_H
