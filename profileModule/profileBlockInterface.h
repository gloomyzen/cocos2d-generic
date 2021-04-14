#ifndef COMMON_PROFILEBLOCKINTERFACE_H
#define COMMON_PROFILEBLOCKINTERFACE_H

#include "rapidjson/document.h"
#include "rapidjson/ostreamwrapper.h"

namespace common {
    namespace profileModule {

        class profileBlockInterface {
          public:
            virtual ~profileBlockInterface() = default;
            virtual bool load(const rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstObject&) = 0;
            virtual bool save(rapidjson::Value&, rapidjson::Document::AllocatorType&) = 0;
        };
    }// namespace profileModule
}// namespace common

#endif// COMMON_PROFILEBLOCKINTERFACE_H
