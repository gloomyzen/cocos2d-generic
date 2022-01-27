#ifdef GetObject
#undef GetObject
#endif
#ifdef ADXE_VERSION // for adxe engine
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/pointer.h"
#else // for original cocos2d-x
#include "json/document.h"
#include "json/istreamwrapper.h"
#include "json/ostreamwrapper.h"
#include "json/stringbuffer.h"
#include "json/writer.h"
#include "json/pointer.h"
#endif

typedef rapidjson::GenericValue<rapidjson::UTF8<char>>::Object              jsonObject;
typedef const rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstObject&  jsonConstObject;
typedef rapidjson::GenericValue<rapidjson::UTF8<char>>::Array               jsonArray;
typedef rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstArray          jsonConstArray;
