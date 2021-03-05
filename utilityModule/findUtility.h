#ifndef COMMON_FINDUTILITY_H
#define COMMON_FINDUTILITY_H

#include "randomUtility.h"
#include <tuple>
#include <vector>

namespace common {
    namespace utilityModule {

        class findUtility {
          public:
            template<typename T>
            static T findClosest(int roll, std::vector<std::pair<int, T>> items) {
                if (roll > 100)
                    roll = 100;
                if (roll <= 0)
                    roll = 1;

                int res = 0, diff = 0, lastIndex = 0;
                for (std::size_t i = 0; i < items.size(); ++i) {
                    if (i == 0 || std::abs(roll - items[i].first) < diff) {
                        if (res == items[i].first) {
                            if (randomUtility::generateBetween(1, 2) == 1)
                                lastIndex = i;
                        } else {
                            lastIndex = i;
                        }
                        res = items[i].first;
                        diff = std::abs(roll - items[i].first);
                    }
                }

                if (res != 0) {
                    return items[lastIndex].second;
                }

                return nullptr;
            }

            template<typename T>
            static int findClosestBetween(T current, T resolution, T step) {
                int cntSteps = 0;
                if (current > resolution) {
                    while (resolution + step <= current) {
                        resolution += step;
                        cntSteps++;
                    }
                } else {
                    while (resolution >= current + step) {
                        current += step;
                        cntSteps++;
                    }
                }
                return cntSteps;
            };
        };
    }// namespace utilityModule
}// namespace common

#endif// COMMON_FINDUTILITY_H
