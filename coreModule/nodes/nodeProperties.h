#pragma once
#ifndef GENERIC_NODEPROPERTIES_H
#define GENERIC_NODEPROPERTIES_H

#include "axmol.h"
#include "generic/coreModule/nodes/nodeFactory.h"
#include "generic/coreModule/resources/resourceManager.h"
#include "generic/coreModule/resources/settings/settingManager.h"
#include "generic/utilityModule/logManager.h"
#include "generic/utilityModule/jsonHelper.h"
#include <string_view>

namespace generic::coreModule {

    class nodeProperties {
    public:
        ~nodeProperties();
        /***
         * Parsing parameters from json file
         * Creating child structure and then read the parameters of each node
         * @param path relative path to file
         * @param node Node instance
         */
        void initWithProperties(const std::string_view& path, ax::Node* node = nullptr);

        /***
         * Parsing parameters from json file
         * But only for first founded node by name
         * @param node Node instance
         * @param name
         */
        void loadProperty(ax::Node* node, const std::string_view& name = "");
        void removeJsonData();
        void loadJson(const std::string_view&);

        bool hasPropertyObject(const std::string_view& name) const;
        bool hasPropertyArray(const std::string_view& name) const;
        template<typename T>
        bool hasProperty(const std::string_view& name) const {
            if (propertyData.HasParseError() || !propertyData.IsObject()) {
                return false;
            }
            if (!propertyData.HasMember("props") || !propertyData["props"].IsObject()) {
                return false;
            }
            if (propertyData["props"].HasMember(name.data())) {
                auto find = propertyData["props"].GetObject().FindMember(name.data());
                if (find != propertyData["props"].GetObject().MemberEnd()) {
                    return find->value.Is<T>();
                }
            }
            return false;
        }

        const jsonObject getPropertyObject(const std::string_view& name);
        const jsonArray getPropertyArray(const std::string_view& name);
        template<typename T>
        const T& getProperty(const std::string_view& name) const {
            assert(hasProperty<T>(name) && "Can't find property, use method 'hasProperty<T>' first!");
            auto obj = propertyData["props"].FindMember(name.data());
            static auto result = obj->value.Get<T>();
            return result;
        }

        void setSettingsData(const rapidjson::GenericValue<rapidjson::UTF8<char>>::Object& object);
        rapidjson::Value getSettingsData();
        void removeSettingsData();
        virtual void updateSettings();


    private:
        void parseProperties(ax::Node* node, const std::string_view& name = "");
        void parseData(ax::Node* node, const rapidjson::GenericValue<rapidjson::UTF8<char>>::Array& array);

        rapidjson::Value settingsData;
        rapidjson::Document propertyData;
        std::string pathProperties;

        static std::string defaultNodesPath;
    };

    template<typename T>
    class nodeWithProperties
      : public T
      , public nodeProperties {};

}// namespace generic::coreModule

#endif// GENERIC_NODEPROPERTIES_H
