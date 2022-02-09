#ifndef GENERIC_JSONLOADER_H
#define GENERIC_JSONLOADER_H

#include "generic/utilityModule/jsonHelper.h"
#include <fstream>
#include <string>

namespace generic::coreModule {
    class jsonLoader {
    public:

        /**
         * Method for load unparsed json, as is, with is object check
         * @param path Path to json in resources folder
         * @param prefix @only for gtest
         * @return rapidjson::Document
         */
        rapidjson::Document loadJson(const std::string& path);
        rapidjson::Document stringToJson(const std::string& str);
        void mergeJson(rapidjson::Value& a, rapidjson::Value& b, rapidjson::Value::AllocatorType& allocator);
        bool mergeObjects(rapidjson::Value& a, rapidjson::Value& b, rapidjson::Document::AllocatorType& allocator);

    private:
        rapidjson::Document findByResolution(const std::string& path);
    };
}// namespace generic::coreModule


#endif// GENERIC_JSONLOADER_H
