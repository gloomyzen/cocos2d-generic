#ifndef COMMON_PROFILEBLOCKINTERFACE_H
#define COMMON_PROFILEBLOCKINTERFACE_H

#include "json/document.h"
#include "json/ostreamwrapper.h"

namespace common {
	namespace profileModule {

		class profileBlockInterface {
		public:
			virtual ~profileBlockInterface() = default;
			virtual bool load(const rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstObject &) = 0;
			virtual bool save(rapidjson::Value&, rapidjson::Document::AllocatorType&) = 0;
		};
	}
}

#endif //COMMON_PROFILEBLOCKINTERFACE_H
