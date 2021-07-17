#ifndef GENERIC_NODEPROPERTIES_H
#define GENERIC_NODEPROPERTIES_H

#include "cocos2d.h"
#include "generic/coreModule/gameManager.h"
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
         * Парсинг параметров из json файла
         * Полный парсинг нод и следом параметров каждой ноды
         * @param path relative path to file
         * @param node Node instance
         */
        void loadProperty(const std::string& path, cocos2d::Node* node = nullptr);

        /***
         * Парсинг параметров из json файла
         * Парсинг только параметров одной ноды
         * @param node Node instance
         * @param name
         */
        void loadComponent(cocos2d::Node* node, const std::string& name = std::string());
        void removeJsonData();
        void loadJson(const std::string&);
        bool hasPropertyObject(const std::string& name);
        rapidjson::GenericValue<rapidjson::UTF8<char>>::Object getPropertyObject(const std::string& name);

        void setSettingsData(const rapidjson::GenericValue<rapidjson::UTF8<char>>::Object& object);
        rapidjson::Value getSettingsData();
        void removeSettingsData();


    private:
        void parseComponents(cocos2d::Node* node, const std::string& name = std::string());
        void parseData(cocos2d::Node* node, const rapidjson::GenericValue<rapidjson::UTF8<char>>::Array& array);

        rapidjson::Value settingsData;
        rapidjson::Document propertyData;
        std::string pathProperties;
    };

    template<typename T>
    class nodeWithProperties : public T, public nodeProperties {};

}// namespace generic::coreModule

#endif// GENERIC_NODEPROPERTIES_H
