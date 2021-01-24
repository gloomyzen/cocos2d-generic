#ifndef SWIPE_RPG_SPRITEPARAMETERS_H
#define SWIPE_RPG_SPRITEPARAMETERS_H

#include "cocos2d.h"

namespace common {

	namespace coreModule {

	class spriteParameters {
		public:
			static void setCorrectPixelartTexture(cocos2d::Sprite* sprite);

			static void reinitWithPolygon(cocos2d::Sprite* sprite, const std::string& filepath, float jd = 2.0f);
		};
	}
}

#endif //SWIPE_RPG_SPRITEPARAMETERS_H
