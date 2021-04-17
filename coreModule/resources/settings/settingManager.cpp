#include "settingManager.h"
#include "common/debugModule/logManager.h"
#include "common/utilityModule/findUtility.h"
#include "rapidjson/document.h"
#include "rapidjson/ostreamwrapper.h"

using namespace common::coreModule;

settingManager::settingManager() = default;

settingManager::~settingManager() {
    for (auto [_, c] : allResolutions) {
        delete c;
        c = nullptr;
    }
    allResolutions.clear();
    delete currentSize;
    currentSize = nullptr;
}

void settingManager::load() {
    auto defaultResolution = new sDisplaySize("default", 1024.f, 768.f, 1.f);

    // load json
    const std::string& regionStr = cocos2d::FileUtils::getInstance()->getStringFromFile("config/setting.json");
    rapidjson::Document doc;
    doc.Parse<0>(regionStr.c_str());

    if (doc.HasParseError() || doc.IsNull()) {
        LOG_ERROR("settingManager::load: json parse error");
        allResolutions.insert({ defaultResolution->resolutionName, defaultResolution });
        return;
    }

    for (auto it = doc.GetObject().begin(); it != doc.GetObject().end(); ++it) {
        auto resName = it->name.GetString();
        auto currentResolution = new sDisplaySize();
        currentResolution->resolutionName = resName;
        for (auto resIt = it->value.GetObject().begin(); resIt != it->value.GetObject().end(); ++resIt) {
            if (!resIt->name.IsString()) {
                continue;
            }
            std::string key = resIt->name.GetString();
            if (key == "width" && resIt->value.IsNumber()) {
                currentResolution->size.width = static_cast<float>(resIt->value.GetInt());
            } else if (key == "height" && resIt->value.IsNumber()) {
                currentResolution->size.height = static_cast<float>(resIt->value.GetInt());
            } else if (key == "desktopScale" && resIt->value.IsNumber()) {
                currentResolution->scale = resIt->value.GetFloat();
            } else if (key == "parent" && resIt->value.IsString()) {
                currentResolution->parentName = resIt->value.GetString();
            } else if (key == "path" && resIt->value.IsString()) {
                currentResolution->path = resIt->value.GetString();
            } else if (key == "showDisplayStats" && resIt->value.IsBool()) {
#ifdef DEBUG
                currentResolution->showStats = resIt->value.GetBool();
#else
                currentResolution->showStats = false;
#endif
            } else if (key == "pixel" && resIt->value.IsBool()) {
                currentResolution->spritePixel = resIt->value.GetBool();
            }
        }
        allResolutions.insert({ resName, currentResolution });
    }

    if (allResolutions.empty()) {
        LOG_ERROR("settingManager::load: no data in json file!");
        allResolutions.insert({ defaultResolution->resolutionName, defaultResolution });
        return;
    } else {
        for (auto& item : allResolutions) {
            if (item.second->parent == nullptr && !item.second->parentName.empty()) {
                auto parent = getSizeByName(item.second->parentName);
                if (parent != nullptr) {
                    item.second->parent = parent;
                }
            }
        }
    }
}

sDisplaySize* settingManager::getSizeByName(const std::string& name) {
    if (name.empty())
        return nullptr;
    auto it = allResolutions.find(name);
    if (it != allResolutions.end())
        return it->second;
    return nullptr;
}

void settingManager::init(bool isMobile, const std::string& settingName) {
    if (isMobile) {
        using namespace common::utilityModule;
        auto director = cocos2d::Director::getInstance();
        auto glView = director->getOpenGLView();
        auto currentRes = glView->getDesignResolutionSize();
        auto current = currentRes.height / currentRes.width;
        int lastSteps = 0;
        for (const auto& item : allResolutions) {
            auto frame = item.second->size.height / item.second->size.width;
            if (currentSize == nullptr) {
                currentSize = item.second;
                lastSteps = findUtility::findClosestBetween(current, frame, 0.1f);
            } else {
                auto newStep = findUtility::findClosestBetween(current, frame, 0.1f);
                if (newStep < lastSteps) {
                    currentSize = item.second;
                    lastSteps = newStep;
                }
            }
        }
    }
    if (!settingName.empty()) {
        auto resolution = getSizeByName(settingName);
        if (resolution != nullptr) {
            currentSize = resolution;
        } else {
            LOG_ERROR("settingManager::getCurrentSize: Can't detect valid resolution!");
        }
    }
    if (currentSize == nullptr) {
        LOG_ERROR("settingManager::getCurrentSize: Can't detect valid resolution!");
    }
}

sDisplaySize* settingManager::getCurrentSize() {
    if (currentSize != nullptr)
        return currentSize;
    return nullptr;
}