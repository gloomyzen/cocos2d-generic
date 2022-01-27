#ifndef GENERIC_NODEPROPERTIES_H
#define GENERIC_NODEPROPERTIES_H

#include "cocos2d.h"
#include "generic/coreModule/nodes/nodeFactory.h"
#include "generic/coreModule/resources/resourceManager.h"
#include "generic/coreModule/resources/settings/settingManager.h"
#include "generic/debugModule/logManager.h"
#include "generic/utilityModule/jsonHelper.h"
#include <string>

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
        void initWithProperties(const std::string& path, cocos2d::Node* node = nullptr);

        /***
         * Parsing parameters from json file
         * But only for first founded node by name
         * @param node Node instance
         * @param name
         */
        void loadProperty(cocos2d::Node* node, const std::string& name = "");
        void removeJsonData();
        void loadJson(const std::string&);

        bool hasPropertyObject(const std::string& name) const;
        bool hasPropertyArray(const std::string& name) const;
        template<typename T>
        bool hasProperty(const std::string& name) const {
            if (propertyData.HasParseError() || !propertyData.IsObject()) {
                return false;
            }
            if (!propertyData.HasMember("props") || !propertyData["props"].IsObject()) {
                return false;
            }
            if (propertyData["props"].HasMember(name.c_str())) {
                auto find = propertyData["props"].GetObject().FindMember(name.c_str());
                if (find != propertyData["props"].GetObject().MemberEnd()) {
                    return find->value.Is<T>();
                }
            }
            return false;
        }

        const jsonObject & getPropertyObject(const std::string& name);
        const jsonArray& getPropertyArray(const std::string& name);
        template<typename T>
        const T& getProperty(const std::string& name) const {
            assert(hasProperty<T>(name) && "Can't find property, use method 'hasProperty<T>' first!");
            auto obj = propertyData["props"].FindMember(name.c_str());
            static auto result = obj->value.Get<T>();
            return result;
        }

        void setSettingsData(const rapidjson::GenericValue<rapidjson::UTF8<char>>::Object& object);
        rapidjson::Value getSettingsData();
        void removeSettingsData();
        virtual void updateSettings();


    private:
        void parseProperties(cocos2d::Node* node, const std::string& name = "");
        void parseData(cocos2d::Node* node, const rapidjson::GenericValue<rapidjson::UTF8<char>>::Array& array);

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
