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

namespace common {
    namespace debugModule {
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

            bool init() override {
                if (!Layer::init()) {
                    return false;
                }

                // General
                classList[typeid(cocos2d::Camera).name()] = "[Camera]";
                classList[typeid(cocos2d::Scene).name()] = "[Scene]";
                // Nodes
                classList[typeid(cocos2d::Node).name()] = "[Node]";
                classList[typeid(cocos2d::ClippingNode).name()] = "[ClippingNode]";
                classList[typeid(cocos2d::DrawNode).name()] = "[DrawNode]";
                classList[typeid(cocos2d::ParticleSystem).name()] = "[ParticleSystem]";
                classList[typeid(cocos2d::ParticleBatchNode).name()] = "[ParticleBatchNode]";
                classList[typeid(cocos2d::SpriteBatchNode).name()] = "[SpriteBatchNode]";
                classList[typeid(cocos2d::ParallaxNode).name()] = "[ParallaxNode]";
                classList[typeid(cocos2d::RenderTexture).name()] = "[RenderTexture]";
                classList[typeid(cocos2d::ClippingRectangleNode).name()] = "[ClippingRectangleNode]";
                classList[typeid(cocos2d::AttachNode).name()] = "[AttachNode]";
                classList[typeid(cocos2d::Sprite).name()] = "[Sprite]";
                classList[typeid(cocos2d::NodeGrid).name()] = "[NodeGrid]";
                classList[typeid(cocos2d::Label).name()] = "[Label]";
                classList[typeid(cocos2d::Menu).name()] = "[Menu]";
                classList[typeid(cocos2d::MenuItem).name()] = "[MenuItem]";
                // todo add other menu types

                // Widgets
                classList[typeid(cocos2d::ui::Widget).name()] = "[ui::Widget]";
                classList[typeid(cocos2d::ui::Layout).name()] = "[ui::Layout]";
                classList[typeid(cocos2d::ui::Button).name()] = "[ui::Button]";
                classList[typeid(cocos2d::ui::ListView).name()] = "[ui::ListView]";
                classList[typeid(cocos2d::ui::AbstractCheckButton).name()] = "[ui::AbstractCheckButton]";
                classList[typeid(cocos2d::ui::CheckBox).name()] = "[ui::CheckBox]";
                classList[typeid(cocos2d::ui::RadioButton).name()] = "[ui::RadioButton]";
                classList[typeid(cocos2d::ui::RadioButtonGroup).name()] = "[ui::RadioButtonGroup]";
                classList[typeid(cocos2d::ui::PageView).name()] = "[ui::PageView]";
                classList[typeid(cocos2d::ui::ImageView).name()] = "[ui::ImageView]";
                classList[typeid(cocos2d::ui::ScrollView).name()] = "[ui::ScrollView]";
                classList[typeid(cocos2d::ui::Slider).name()] = "[ui::Slider]";
                classList[typeid(cocos2d::ui::EditBox).name()] = "[ui::EditBox]";
                classList[typeid(cocos2d::ui::LoadingBar).name()] = "[ui::LoadingBar]";
                classList[typeid(cocos2d::ui::Text).name()] = "[ui::Text]";
                classList[typeid(cocos2d::ui::TextField).name()] = "[ui::TextField]";
                classList[typeid(cocos2d::ui::RichText).name()] = "[ui::RichText]";
                classList[typeid(cocos2d::ui::TabControl).name()] = "[ui::TabControl]";
                classList[typeid(cocos2d::ui::TabHeader).name()] = "[ui::TabHeader]";
                // 3D & Light
                classList[typeid(cocos2d::Sprite3D).name()] = "[Sprite3D]";
                classList[typeid(cocos2d::BillBoard).name()] = "[BillBoard]";
                classList[typeid(cocos2d::Skybox).name()] = "[Skybox]";
                classList[typeid(cocos2d::Mesh).name()] = "[Mesh]";
                classList[typeid(cocos2d::Terrain).name()] = "[Terrain]";
                classList[typeid(cocos2d::AmbientLight).name()] = "[AmbientLight]";
                classList[typeid(cocos2d::BaseLight).name()] = "[BaseLight]";
                classList[typeid(cocos2d::DirectionLight).name()] = "[DirectionLight]";
                classList[typeid(cocos2d::PointLight).name()] = "[PointLight]";
                classList[typeid(cocos2d::SpotLight).name()] = "[SpotLight]";
                // Layers
                classList[typeid(cocos2d::Layer).name()] = "[Layer]";
                classList[typeid(cocos2d::LayerColor).name()] = "[LayerColor]";
                classList[typeid(imGuiLayer).name()] = "[imGuiLayer]";
                // Actions
                classList[typeid(cocos2d::Action).name()] = "[Action]";
                // TODO add DragonBones node
                classList[typeid(dragonBones::CCArmatureDisplay).name()] = "[DBArmature]";
                classList[typeid(common::coreModule::armatureNode).name()] = "[DBHolder]";

                _onStart();
                scheduleUpdate();

                return true;
            }


            float getStageWidth() const {
                const auto& stageSize = cocos2d::Director::getInstance()->getVisibleSize();
                return stageSize.width;
            }

            float getStageHeight() const {
                const auto& stageSize = cocos2d::Director::getInstance()->getVisibleSize();
                return stageSize.height;
            }

            //			void _addArmature()
            //			{
            //				if (_armatures.empty())
            //				{
            //					dragonBones::CCFactory::getFactory()->loadDragonBonesData("mecha_1406/mecha_1406_ske.dbbin");
            //					dragonBones::CCFactory::getFactory()->loadTextureAtlasData("mecha_1406/mecha_1406_tex.json");
            //				}
            //
            //				const auto armatureDisplay =
            //dragonBones::CCFactory::getFactory()->buildArmatureDisplay("mecha_1406"); 				armatureDisplay->setScale(0.3f);
            //				armatureDisplay->setPosition(333.f, 26.f);
            //				armatureDisplay->getArmature()->setCacheFrameRate(24);
            //				armatureDisplay->getAnimation()->play("walk");
            //				addChild(armatureDisplay);
            //
            //				_armatures.push_back(armatureDisplay);
            //			}

          private:
            //			std::vector<dragonBones::CCArmatureDisplay*> _armatures;
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
             * Debug button properties
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
    }// namespace debugModule
}// namespace common

#endif// DEBUG
#endif// COMMON_IMGUILAYER_H
