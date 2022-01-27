#ifndef GENERIC_PROFILEBLOCKINTERFACE_H
#define GENERIC_PROFILEBLOCKINTERFACE_H

#include "generic/utilityModule/jsonHelper.h"

namespace generic::profileModule {

    class profileBlockInterface {
    public:
        virtual ~profileBlockInterface() = default;
        virtual bool load(const rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstObject&) = 0;
        virtual bool save(rapidjson::Value&, rapidjson::Document::AllocatorType&) = 0;
    };
}// namespace generic::profileModule

#endif// GENERIC_PROFILEBLOCKINTERFACE_H
