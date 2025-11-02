#pragma once
#ifndef GENERIC_SPRITEPOLYGON_H
#define GENERIC_SPRITEPOLYGON_H

#include "axmol/axmol.h"
#include "axmol/ui/CocosGUI.h"

namespace generic::coreModule {

    class spritePolygon : public ax::AutoPolygon {
    public:
        spritePolygon(const std::string& filename);
        ~spritePolygon();
        float getWidth();
        float getHeight();
        void setWidth(float value);
        void setHeight(float value);
    };
}// namespace generic::coreModule


#endif// GENERIC_SPRITEPOLYGON_H
