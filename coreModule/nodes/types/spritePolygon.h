#ifndef GENERIC_SPRITEPOLYGON_H
#define GENERIC_SPRITEPOLYGON_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace generic {

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
}// namespace generic


#endif// GENERIC_SPRITEPOLYGON_H
