#ifndef COMMON_DATABASEINTERFACE_H
#define COMMON_DATABASEINTERFACE_H

#include "cocos2d.h"
#include "common/debugModule/logManager.h"
#include "rapidjson/document.h"
#include "rapidjson/ostreamwrapper.h"
#include <string>

namespace common::databaseModule {
    class databaseInterface {
      public:
        databaseInterface() = default;
        virtual ~databaseInterface() = default;

        virtual void executeLoadData() {
            if (isLoaded())
                return;
            const std::string& jsonStr = cocos2d::FileUtils::getInstance()->getStringFromFile(jsonPath);
            rapidjson::Document data;
            data.Parse<0>(jsonStr.c_str());
            if (isValidJson(data)) {
                load(data);
                setLoaded(true);
            }
        }

        virtual void load(const rapidjson::Document&) = 0;

        bool isValidJson(const rapidjson::Document& data) {
            if (data.HasParseError()) {
                LOG_ERROR("isValidJson: json parse error");
                return false;
            }

            if (data.IsNull()) {
                LOG_ERROR("isValidJson: json is empty");
                return false;
            }
            return true;
        }

        const std::string& getPath() const { return jsonPath; }

        void setPath(const std::string& path) { jsonPath = path; }

        bool isLoaded() const { return loaded; }

      protected:
        void setLoaded(bool _loaded) { loaded = _loaded; }

      private:
        std::string jsonPath;
        bool loaded = false;
    };
}// namespace common::databaseModule

#endif// COMMON_DATABASEINTERFACE_H
