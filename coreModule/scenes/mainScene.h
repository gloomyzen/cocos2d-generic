#ifndef COMMON_MAINSCENE_H
#define COMMON_MAINSCENE_H

#include "cocos2d.h"
#include "common/coreModule/nodes/nodeProperties.h"
#include "common/coreModule/scenes/windows/windowSystem.h"
#include <string>
#include <vector>
#include "common/debugModule/imGuiLayer.h"

namespace common {
    namespace coreModule {

        class mainScene : public cocos2d::Scene {
          public:
            mainScene();
            ~mainScene();
            static cocos2d::Scene* createScene();

            virtual bool init();

            void setRoom(const std::string&);

            CREATE_FUNC(mainScene);

            windowSystem* getWindowNode() { return windowViewer; }

#ifdef DEBUG
            common::debugModule::imGuiLayer* getImGuiLayer() { return imGuiLayer; }
#endif
          private:
            void initTaskLoading(cocos2d::Layer* layer);

            std::vector<cocos2d::Node*> nodes{};
            windowSystem* windowViewer = nullptr;

#ifdef DEBUG
            common::debugModule::imGuiLayer* imGuiLayer = nullptr;
#endif
        };
    }// namespace coreModule
}// namespace common


#endif// COMMON_MAINSCENE_H
