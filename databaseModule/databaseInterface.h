#pragma once
#ifndef GENERIC_DATABASEINTERFACE_H
#define GENERIC_DATABASEINTERFACE_H

#include "axmol.h"
#include "generic/utilityModule/logManager.h"
#include "generic/utilityModule/jsonHelper.h"
#include <string>

namespace generic::databaseModule {
    class databaseInterface {
    public:
        databaseInterface() = default;
        virtual ~databaseInterface() = default;

        virtual void executeLoadData() {
            if (isLoaded())
                return;
            const std::string& jsonStr = ax::FileUtils::getInstance()->getStringFromFile(jsonPath);
            rapidjson::Document data;
            data.Parse<0>(jsonStr.c_str());
            if (utilityModule::isValidJson(data)) {
                load(data);
                setLoaded(true);
            }
        }

        virtual void load(const rapidjson::Document&) = 0;

        const std::string& getPath() const {
            return jsonPath;
        }

        void setPath(const std::string& path) {
            jsonPath = path;
        }

        bool isLoaded() const {
            return loaded;
        }

    protected:
        void setLoaded(bool _loaded) {
            loaded = _loaded;
        }

    private:
        std::string jsonPath;
        bool loaded = false;
    };
}// namespace generic::databaseModule

#endif// GENERIC_DATABASEINTERFACE_H
