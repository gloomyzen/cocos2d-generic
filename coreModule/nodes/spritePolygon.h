#ifndef COMMON_SPRITEPOLYGON_H
#define COMMON_SPRITEPOLYGON_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace common {

    namespace coreModule {

        class spritePolygon : public cocos2d::AutoPolygon {
          public:
            spritePolygon(const std::string& filename);
            ~spritePolygon();
            float getWidth();
            float getHeight();
            void setWidth(float value);
            void setHeight(float value);
        };
    }// namespace coreModule
}// namespace common


#endif// COMMON_SPRITEPOLYGON_H
