#ifndef COMMON_SETTINGMANAGER_H
#define COMMON_SETTINGMANAGER_H

#include "cocos2d.h"

namespace common {
	namespace coreModule {
		class settingManager {
		private:
			settingManager(cocos2d::Size frameResolution, const cocos2d::Size largeResolution, const bool showDisplayStats);

		public:

			static settingManager load();

			const cocos2d::Size frameResolutionSize;
			const cocos2d::Size largeResolutionSize;
			const bool showDisplayStats;
		};
	}
}


#endif //COMMON_SETTINGMANAGER_H
