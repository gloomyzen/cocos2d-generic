#ifndef COMMON_IMGUILAYER_H
#define COMMON_IMGUILAYER_H

#ifdef DEBUG

#include "CCImGuiLayer.h"
#include "cocos2d.h"
#include "common/coreModule/nodes/widgets/armatureNode.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"
#include "ui/CocosGUI.h"
#include <CCIMGUI.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <list>
#include <vector>

namespace common::debugModule {
    class imGuiLayer : public cocos2d::Layer {
      protected:
        void _onStart();

        void update(float dt) override {
            if (!this->isVisible()) {
                this->setVisible(true);
            }
        }

      public:
        imGuiLayer() {}
        ~imGuiLayer() = default;

        CREATE_FUNC(imGuiLayer);

        bool init() override;

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
    };
}// namespace common::debugModule

#endif// DEBUG
#endif// COMMON_IMGUILAYER_H
