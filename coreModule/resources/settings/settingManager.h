#ifndef COMMON_SETTINGMANAGER_H
#define COMMON_SETTINGMANAGER_H

#include "cocos2d.h"

namespace common {
	namespace coreModule {

		struct sDisplaySize {
			cocos2d::Size size;
			float scale;
			sDisplaySize(float width, float height, float desktopScale) {
				size.width = width;
				size.height = height;
				scale = desktopScale;
			}
		};

		class settingManager {
		private:
			settingManager(const sDisplaySize frameResolution, const sDisplaySize largeResolution, const bool showDisplayStats);

		public:

			static settingManager load();

			const sDisplaySize frameResolutionSize;
			const sDisplaySize largeResolutionSize;
			const bool showDisplayStats;
		};
	}
}


#endif //COMMON_SETTINGMANAGER_H
