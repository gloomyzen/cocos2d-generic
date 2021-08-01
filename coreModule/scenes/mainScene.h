#ifndef GENERIC_MAINSCENE_H
#define GENERIC_MAINSCENE_H

#include "cocos2d.h"
#include "generic/coreModule/nodes/nodeProperties.h"
#include "generic/coreModule/scenes/windows/windowSystem.h"
#include "generic/debugModule/imGuiLayer.h"
#include "ui/CocosGUI.h"
#include <string>
#include <vector>

namespace generic {
    namespace coreModule {

        class mainScene : public cocos2d::Scene {
        public:
            mainScene();
            ~mainScene();
            static cocos2d::Scene* createScene();
            virtual bool init();
            void setRoom(const std::string&);
            CREATE_FUNC(mainScene);
            windowSystem* getWindowNode() {
                return windowViewer;
            }

#ifdef DEBUG
            generic::debugModule::imGuiLayer* getImGuiLayer() {
                return imGuiLayer;
            }
#endif
        private:
            void initTaskLoading(cocos2d::Node* node);

            std::vector<cocos2d::Node*> nodes{};
            windowSystem* windowViewer = nullptr;

#ifdef DEBUG
            generic::debugModule::imGuiLayer* imGuiLayer = nullptr;
#endif
        };
    }// namespace coreModule
}// namespace generic


#endif// GENERIC_MAINSCENE_H
