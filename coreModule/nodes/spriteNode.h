#ifndef SWIPE_RPG_SPRITENODE_H
#define SWIPE_RPG_SPRITENODE_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace common {

	namespace coreModule {

		class spriteNode : public cocos2d::Sprite {
		public:
			cocos2d::Sprite *createSpritePolygon(const std::string &, float jd = 2.0f);
		};
	}
}


#endif //SWIPE_RPG_SPRITENODE_H
