#pragma once
#ifndef GENERIC_SETTINGMANAGER_H
#define GENERIC_SETTINGMANAGER_H

#include "axmol.h"
#include <map>
#include <string>
#include <utility>

namespace generic {
    namespace coreModule {

        struct sDisplaySize {
            std::string resolutionName;
            ax::Size size;
            float scale = 1.f;
            std::string path;
            std::string parentName;
            std::shared_ptr<sDisplaySize> parent = nullptr;
            bool showStats = false;
            bool spritePixel = false;
            sDisplaySize() = default;
            sDisplaySize(std::string name, float width, float height, float desktopScale) {
                resolutionName = std::move(name);
                size.width = width;
                size.height = height;
                scale = desktopScale;
            }
            std::string getPath() {
                std::string fullPath;
                if (parent != nullptr) {
                    fullPath += parent->getPath();
                }
                fullPath += path;
                return fullPath;
            }
        };

        class settingManager {
        public:
            settingManager();
            ~settingManager();
            void load();
            bool init(bool isMobile, const std::string& settingName = "");
            std::shared_ptr<sDisplaySize> getCurrentSize();

        private:
            std::shared_ptr<sDisplaySize> getSizeByName(const std::string&);
            std::map<std::string, std::shared_ptr<sDisplaySize>> allResolutions;
            std::shared_ptr<sDisplaySize> currentSize;
        };
    }// namespace coreModule
}// namespace generic


#endif// GENERIC_SETTINGMANAGER_H
