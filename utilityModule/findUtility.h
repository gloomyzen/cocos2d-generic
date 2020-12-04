#ifndef SWIPE_RPG_FINDUTILITY_H
#define SWIPE_RPG_FINDUTILITY_H

#include <tuple>
#include <vector>

namespace common {
	namespace utilityModule {

		class findUtility {
		public:
			template <typename T>
			static T findClosest(int roll, std::vector<std::pair<int, T>> items) {
				if (roll > 100) roll = 100;
				if (roll <= 0) roll = 1;

				int res = 0, diff = 0, lastIndex = 0;
				for (std::size_t i = 0; i < items.size(); ++i) {
					if (i == 0 || std::abs(roll - items[i].first) < diff) {
						//todo random from res
//						if (res == items[i]) {}
						res = items[i].first;
						lastIndex = i;
						diff = std::abs(roll - items[i].first);
					}
				}

				if (res != 0) {
					return items[lastIndex].second;
				}

				return T();
			}
		};
	}
}

#endif //SWIPE_RPG_FINDUTILITY_H
