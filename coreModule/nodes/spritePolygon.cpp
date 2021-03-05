#include "spritePolygon.h"

using namespace common::coreModule;

spritePolygon::spritePolygon(const std::string& filename)
    : AutoPolygon(filename){

      };

spritePolygon::~spritePolygon() {}

float spritePolygon::getWidth() { return static_cast<float>(_width); };

float spritePolygon::getHeight() { return static_cast<float>(_height); }

void spritePolygon::setWidth(float value) { _width = static_cast<unsigned>(value); }

void spritePolygon::setHeight(float value) { _height = static_cast<unsigned>(value); };