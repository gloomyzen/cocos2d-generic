#ifndef SWIPE_RPG_SPRITEPOLYGON_H
#define SWIPE_RPG_SPRITEPOLYGON_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace common {

	namespace coreModule {

		class spritePolygon : public cocos2d::AutoPolygon {
		public:
			spritePolygon(const std::string &filename);
			~spritePolygon();
			float getWidth();
			float getHeight();
			void setWidth(float value);
			void setHeight(float value);

		};
	}
}


#endif //SWIPE_RPG_SPRITEPOLYGON_H
