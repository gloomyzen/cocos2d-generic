#ifndef COMMON_IMGUILAYER_H
#define COMMON_IMGUILAYER_H

#ifdef DEBUG

#include "DragonBones/CCDragonBonesHeaders.h"
#include "ImGuiEXT/CCImGuiEXT.h"
#include "ImGuiEXT/imgui/imgui.h"
#include "ImGuiEXT/imgui/imgui_internal.h"
#include "cocos2d.h"
#include "common/coreModule/nodes/widgets/armatureNode.h"
#include "platform/CCPlatformConfig.h"
#include "ui/CocosGUI.h"
#include <functional>
#include <list>
#include <vector>

namespace common::debugModule {
    class imGuiLayer : public cocos2d::Layer {
      protected:
        void update(float dt) override {
            if (!this->isVisible()) {
                this->setVisible(true);
            }
        }

      public:
        imGuiLayer() {}
        ~imGuiLayer() {
            resetDebugModules();
            classList.clear();
        };

        CREATE_FUNC(imGuiLayer);

        bool init() override;

        void resetDebugModules();

        void addDebugModules(std::function<void()>);

        void drawImgui();

      private:
        /// Debug data
        void debugToggleRow(Node*);
        /// Node editor
        std::map<std::string, std::string> classList;
        void showNodeEditor(bool* p_open);
        ImRect renderTree(cocos2d::Vector<Node*> n);
        ImRect renderPreferences(Node*);


        bool m_enabled = true;
        float default_width = 0.0f;
        /***
         * Debug buttonNode properties
         */
        int debugBtnW = 65;
        int debugBtnH = 80;

        /***
         * Node editor window
         */
        int nodeEditorW = 430;
        int nodeEditorH = 450;
        Node* lastTarget = nullptr;
        std::vector<std::function<void()>> debugModules;
    };
}// namespace common::debugModule

#endif// DEBUG
#endif// COMMON_IMGUILAYER_H
