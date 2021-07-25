#ifndef GENERIC_IMGUILAYER_H
#define GENERIC_IMGUILAYER_H

#ifdef DEBUG

#include "DragonBones/CCDragonBonesHeaders.h"
#include "ImGuiEXT/CCImGuiEXT.h"
#include "ImGuiEXT/imgui/imgui.h"
#include "ImGuiEXT/imgui/imgui_internal.h"
#include "cocos2d.h"
#include "generic/coreModule/nodes/types/armatureNode.h"
#include "platform/CCPlatformConfig.h"
#include "ui/CocosGUI.h"
#include <functional>
#include <list>
#include <vector>
#include <string>
#include <tuple>

namespace generic::debugModule {
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
        void addDebugModules(const std::pair<std::string, std::function<void()>>&);
        void drawImgui();

        static bool Combo(const char* label, int* current_item, const std::vector<std::string>& items, int items_count, int height_in_items = -1);

      private:
        void initEvents();
        /// Debug data
        void debugToggleRow(Node*);
        /// Node editor
        std::map<std::string, std::string> classList;
        void showNodeEditor(bool* editorOpened);
        ImRect renderTree(cocos2d::Vector<Node*> n);
        ImRect renderPreferences(Node*);

        cocos2d::EventListenerKeyboard* keyboardListener = nullptr;

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
        std::vector<std::pair<std::string, std::function<void()>>> debugModules;
    };
}// namespace generic::debugModule

#endif// DEBUG
#endif// GENERIC_IMGUILAYER_H
