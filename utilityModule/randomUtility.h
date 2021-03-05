#ifndef COMMON_RANDOMUTILITY_H
#define COMMON_RANDOMUTILITY_H

#include <random>

namespace common {
    namespace utilityModule {
        class randomUtility {

          public:
            static int generateBetween(int a, int b) {
                std::random_device rnd_device;
                std::mt19937 mt_rand{ rnd_device() };
                std::uniform_int_distribution<int> dist{ a, b };
                return dist(mt_rand);
            }

            template<typename T1, typename T2>
            static T1 getRandomVector(std::vector<T2> array) {
                if (array.empty())
                    return T1();

                int key = randomUtility::generateBetween(0, array.size() - 1);

                try {
                    auto& element = array.at(key);
                    return element;
                } catch (const std::out_of_range& oor) { return getRandomVector<T1>(array); }
            }
        };
    }// namespace utilityModule
}// namespace common

#endif// COMMON_RANDOMUTILITY_H
