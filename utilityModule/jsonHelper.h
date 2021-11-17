#ifdef GetObject
#undef GetObject
#endif
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/pointer.h"

typedef rapidjson::GenericValue<rapidjson::UTF8<char>>::Object              jsonObject;
typedef const rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstObject&  jsonConstObject;
typedef rapidjson::GenericValue<rapidjson::UTF8<char>>::Array               jsonArray;
typedef rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstArray          jsonConstArray;
