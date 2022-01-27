#ifndef GENERIC_LABELPROPERTY_H
#define GENERIC_LABELPROPERTY_H

#include "cocos2d.h"
#include "generic/utilityModule/jsonHelper.h"
#include "propertyInterface.h"
#include <string>
#include <map>

namespace generic::coreModule {
    class labelProperty : public propertyInterface {
    public:
        labelProperty(const std::string& _propertyName) : propertyInterface(_propertyName) {}

        void parseProperty(cocos2d::Node* node, const jsonObject& object) override;

    private:
        static std::string defaultFont;
        static std::map<std::string, cocos2d::TextHAlignment> textHAlignmentTypes;
    };
}// namespace generic::coreModule


#endif// GENERIC_LABELPROPERTY_H
