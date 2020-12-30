#ifndef COMMON_SETTINGMANAGER_H
#define COMMON_SETTINGMANAGER_H

#include "cocos2d.h"
#include <string>
#include <vector>

namespace common {
	namespace coreModule {

		struct sDisplaySize {
			cocos2d::Size size;
			float scale;
			std::string path;
			sDisplaySize* parent = nullptr;
			sDisplaySize(float width, float height, float desktopScale) {
				size.width = width;
				size.height = height;
				scale = desktopScale;
			}
			std::string getPath() {
				std::string fullPath;
				if (parent != nullptr) {
					fullPath += getPath();
				}
				fullPath += path;
				return fullPath;
			}
		};

		class settingManager {
		public:
			settingManager();
			void load();

		private:
			std::vector<sDisplaySize*> allResolutions;
		};
	}
}


#endif //COMMON_SETTINGMANAGER_H
