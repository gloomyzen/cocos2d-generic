#ifndef COMMON_JSONLOADER_H
#define COMMON_JSONLOADER_H

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <fstream>
#include <string>

namespace common {

    namespace coreModule {
        class jsonLoader {
          public:
            jsonLoader();
            ~jsonLoader();

            /**
             * Method for load unparsed json, as is, with is object check
             * @param path Path to json in resources folder
             * @param prefix @only for gtest
             * @return rapidjson::Document
             */
            rapidjson::Document loadJson(const std::string& path);
            rapidjson::Document stringToJson(const std::string& str);
            void mergeJson(rapidjson::Value& a, rapidjson::Value& b, rapidjson::Value::AllocatorType& allocator);

          private:
            rapidjson::Document findByResolution(const std::string& path);
        };
    }// namespace coreModule
}// namespace common


#endif// COMMON_JSONLOADER_H
