#ifndef COMMON_SETTINGMANAGER_H
#define COMMON_SETTINGMANAGER_H

#include "cocos2d.h"
#include <string>
#include <utility>
#include <map>

namespace common {
	namespace coreModule {

		struct sDisplaySize {
			std::string resolutionName;
			cocos2d::Size size;
			float scale = 1.f;
			std::string path;
			std::string parentName;
			sDisplaySize* parent = nullptr;
			bool showStats = false;
			sDisplaySize() = default;
			sDisplaySize(std::string name, float width, float height, float desktopScale) {
				resolutionName = std::move(name);
				size.width = width;
				size.height = height;
				scale = desktopScale;
			}
			std::string getPath() {
				std::string fullPath;
				if (parent != nullptr) {
					fullPath += parent->getPath();
				}
				fullPath += path;
				return fullPath;
			}
		};

		class settingManager {
		public:
			settingManager();
			void load();
			void init(bool isMobile, std::string settingName = "");
			sDisplaySize* getCurrentSize();

		private:
			sDisplaySize* getSizeByName(std::string);
			std::map<std::string, sDisplaySize*> allResolutions;
			sDisplaySize* currentSize = nullptr;
		};
	}
}


#endif //COMMON_SETTINGMANAGER_H
