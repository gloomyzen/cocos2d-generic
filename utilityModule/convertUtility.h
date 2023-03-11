#pragma once
#ifndef GENERIC_COVERTUTILITY_H
#define GENERIC_COVERTUTILITY_H

#include "axmol.h"

namespace generic::utilityModule {
    class convertUtility {
    public:
        static ax::Color3B changeColorFromTo(const ax::Color3B& from, const ax::Color3B& to, float percent) {
            if (percent > 1.0f)
                percent = 1.0f;
            else if (percent < 0.f)
                percent = 0.f;
            auto getColor = [](float a, float b, float p) {
                return static_cast<int>((b - a) * p + a);
            };
            ax::Color3B result;
            result.r = getColor(from.r, to.r, percent);
            result.g = getColor(from.g, to.g, percent);
            result.b = getColor(from.b, to.b, percent);
            return result;
        };

        static ax::Color3B changeColorByPercent(const ax::Color3B& color, float percent) {
            if (percent > 1.0f)
                percent = 1.0f;
            else if (percent < 0.f)
                percent = 0.f;
            auto getColor = [](float a, float p) {
                if (a * p >= 255.f)
                    return static_cast<int>(255);
                if (a * p <= 0.f)
                    return static_cast<int>(0);
                return static_cast<int>(a * p);
            };
            ax::Color3B result;
            result.r = getColor(color.r, percent);
            result.g = getColor(color.g, percent);
            result.b = getColor(color.b, percent);
            return result;
        }
    };
}// namespace generic::utilityModule

#endif// GENERIC_COVERTUTILITY_H
