#ifndef SWIPE_RPG_PROFILEBLOCKINTERFACE_H
#define SWIPE_RPG_PROFILEBLOCKINTERFACE_H

#include "json/document.h"
#include "json/ostreamwrapper.h"

namespace common {
	namespace profileModule {

		class profileBlockInterface {
		public:
			virtual ~profileBlockInterface() = default;
			virtual bool load(const rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstObject &) = 0;
			virtual bool save(rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstObject &) = 0;
		};
	}
}

#endif //SWIPE_RPG_PROFILEBLOCKINTERFACE_H
