#ifdef GetObject
#undef GetObject
#endif
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

typedef rapidjson::GenericValue<rapidjson::UTF8<char>>::Object jsonObject;
typedef rapidjson::GenericValue<rapidjson::UTF8<char>>::Object jsonArray;
