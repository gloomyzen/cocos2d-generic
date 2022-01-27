#ifndef GENERIC_JSONHELPER_H
#define GENERIC_JSONHELPER_H
#ifdef GetObject
#undef GetObject
#endif
#include "cocos2d.h"
#ifdef ADXE_VERSION// for adxe engine
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/pointer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#else// for original cocos2d-x
#include "json/document.h"
#include "json/istreamwrapper.h"
#include "json/ostreamwrapper.h"
#include "json/pointer.h"
#include "json/stringbuffer.h"
#include "json/writer.h"
#endif

typedef rapidjson::GenericValue<rapidjson::UTF8<char>>::Object jsonObject;
typedef const rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstObject& jsonConstObject;
typedef rapidjson::GenericValue<rapidjson::UTF8<char>>::Array jsonArray;
typedef rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstArray jsonConstArray;

namespace generic::utilityModule {

    inline bool isValidJson(const rapidjson::Document& data) {
        if (data.HasParseError() || data.IsNull()) {
            return false;
        }
        return true;
    }

}// namespace generic::utilityModule

#endif
