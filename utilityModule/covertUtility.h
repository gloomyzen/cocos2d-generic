#ifndef COMMON_COVERTUTILITY_H
#define COMMON_COVERTUTILITY_H

#include "cocos2d.h"

namespace common::utilityModule {
    class convertUtility {
      public:
        static cocos2d::Color3B changeColor(const cocos2d::Color3B& from, const cocos2d::Color3B& to, float percent) {
            if (percent > 1.0f) percent = 1.0f;
            else if (percent < 0.f) percent = 0.f;
            auto getColor = [](float a, float b, float p) {
                return static_cast<int>((b - a) * p + a);
            };
            cocos2d::Color3B result;
            result.r = getColor(from.r, to.r, percent);
            result.g = getColor(from.g, to.g, percent);
            result.b = getColor(from.b, to.b, percent);
            return result;
        };
    };
}// namespace common::utilityModule

#endif// COMMON_COVERTUTILITY_H
