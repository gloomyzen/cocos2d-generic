#ifndef COMMON_NODEPROPERTIES_H
#define COMMON_NODEPROPERTIES_H

#include "cocos2d.h"
#include "common/coreModule/gameManager.h"
#include "common/coreModule/nodes/nodeFactory.h"
#include "common/coreModule/resources/resourceManager.h"
#include "common/coreModule/resources/settings/settingManager.h"
#include "common/debugModule/logManager.h"
#include "rapidjson/document.h"
#include <string>

namespace common::coreModule {

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

        const rapidjson::Document& getPropertyData() {
            return propertyData;
        }

      private:

        void parseComponents(cocos2d::Node* node, const std::string& name = std::string());

        void parseData(cocos2d::Node* node, const rapidjson::GenericValue<rapidjson::UTF8<char>>::Array& array);

        rapidjson::Document propertyData;
        std::string pathProperties;

    };

    template<typename T>
    class nodeWithProperties : public T, public nodeProperties {};
}// namespace common::coreModule

#endif// COMMON_NODEPROPERTIES_H
