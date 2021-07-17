#ifndef GENERIC_JSONLOADER_H
#define GENERIC_JSONLOADER_H

#include "generic/utilityModule/jsonHelper.h"
#include <fstream>
#include <string>

namespace generic {

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
}// namespace generic


#endif// GENERIC_JSONLOADER_H
