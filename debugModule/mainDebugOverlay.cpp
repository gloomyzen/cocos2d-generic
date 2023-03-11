#include "mainDebugOverlay.h"
#include "imguiController.h"
#include "propertyViewer/propertyViewer.h"
#include "generic/coreModule/nodes/nodeProperties.h"
#include "spine/spine-cocos2dx.h"
#include "ui/CocosGUI.h"
#include "FairyGUI.h"
#include "fairygui/display/FUISprite.h"
#include <functional>
#include <algorithm>

using namespace generic::debugModule;

bool mainDebugOverlay::m_enabled = true;
float mainDebugOverlay::default_width = 0.0f;
int mainDebugOverlay::nodeEditorW = 520;
int mainDebugOverlay::nodeEditorH = 450;
propertyViewer* mainDebugOverlay::_propertyViewer = nullptr;
ax::Node* mainDebugOverlay::lastTarget = nullptr;
std::vector<std::pair<std::string, std::function<void()>>> mainDebugOverlay::overlayModules = {};
std::map<std::string, std::string> mainDebugOverlay::classList = {};

static bool closeAll = false;
static bool debugOpened = false;
static bool nodeEditorOpened = false;

void mainDebugOverlay::init() {
    if (_propertyViewer)
        return;
    _propertyViewer = new propertyViewer();

    {
        // General
        classList[typeid(ax::Camera).name()] = "[Camera]";
        classList[typeid(ax::Scene).name()] = "[Scene]";
        // Nodes
        classList[typeid(ax::Node).name()] = "[Node]";
        classList[typeid(ax::ClippingNode).name()] = "[ClippingNode]";
        classList[typeid(ax::DrawNode).name()] = "[DrawNode]";
        classList[typeid(ax::ParticleSystem).name()] = "[ParticleSystem]";
        classList[typeid(ax::ParticleBatchNode).name()] = "[ParticleBatchNode]";
        classList[typeid(ax::SpriteBatchNode).name()] = "[SpriteBatchNode]";
        classList[typeid(ax::ParallaxNode).name()] = "[ParallaxNode]";
        classList[typeid(ax::RenderTexture).name()] = "[RenderTexture]";
        classList[typeid(ax::ClippingRectangleNode).name()] = "[ClippingRectangleNode]";
        classList[typeid(ax::AttachNode).name()] = "[AttachNode]";
        classList[typeid(ax::Sprite).name()] = "[Sprite]";
        classList[typeid(ax::NodeGrid).name()] = "[NodeGrid]";
        classList[typeid(ax::Label).name()] = "[Label]";
        classList[typeid(ax::Menu).name()] = "[Menu]";
        classList[typeid(ax::MenuItem).name()] = "[MenuItem]";
        // todo add other menu types

        // Widgets
        classList[typeid(ax::ui::Widget).name()] = "[ui::Widget]";
        classList[typeid(ax::ui::Layout).name()] = "[ui::Layout]";
        classList[typeid(ax::ui::Button).name()] = "[ui::Button]";
        classList[typeid(ax::ui::ListView).name()] = "[ui::ListView]";
        classList[typeid(ax::ui::AbstractCheckButton).name()] = "[ui::AbstractCheckButton]";
        classList[typeid(ax::ui::CheckBox).name()] = "[ui::CheckBox]";
        classList[typeid(ax::ui::RadioButton).name()] = "[ui::RadioButton]";
        classList[typeid(ax::ui::RadioButtonGroup).name()] = "[ui::RadioButtonGroup]";
        classList[typeid(ax::ui::PageView).name()] = "[ui::PageView]";
        classList[typeid(ax::ui::ImageView).name()] = "[ui::ImageView]";
        classList[typeid(ax::ui::ScrollView).name()] = "[ui::ScrollView]";
        classList[typeid(ax::ui::Slider).name()] = "[ui::Slider]";
        classList[typeid(ax::ui::EditBox).name()] = "[ui::EditBox]";
        classList[typeid(ax::ui::LoadingBar).name()] = "[ui::LoadingBar]";
        classList[typeid(ax::ui::Text).name()] = "[ui::Text]";
        classList[typeid(ax::ui::TextField).name()] = "[ui::TextField]";
        classList[typeid(ax::ui::RichText).name()] = "[ui::RichText]";
        classList[typeid(ax::ui::TabControl).name()] = "[ui::TabControl]";
        classList[typeid(ax::ui::TabHeader).name()] = "[ui::TabHeader]";
        // 3D & Light
        classList[typeid(ax::Sprite3D).name()] = "[Sprite3D]";
        classList[typeid(ax::BillBoard).name()] = "[BillBoard]";
        classList[typeid(ax::Skybox).name()] = "[Skybox]";
        classList[typeid(ax::Mesh).name()] = "[Mesh]";
        classList[typeid(ax::Terrain).name()] = "[Terrain]";
        classList[typeid(ax::AmbientLight).name()] = "[AmbientLight]";
        classList[typeid(ax::BaseLight).name()] = "[BaseLight]";
        classList[typeid(ax::DirectionLight).name()] = "[DirectionLight]";
        classList[typeid(ax::PointLight).name()] = "[PointLight]";
        classList[typeid(ax::SpotLight).name()] = "[SpotLight]";
        // Layers
        classList[typeid(ax::Layer).name()] = "[Layer]";
        classList[typeid(ax::LayerColor).name()] = "[LayerColor]";
        // Actions
        classList[typeid(ax::Action).name()] = "[Action]";
        // DragonBones node
#if defined(AX_BUILD_WITH_DRANGBONES) && AX_BUILD_WITH_DRANGBONES
        classList[typeid(dragonBones::CCArmatureDisplay).name()] = "[DBArmature]";
        classList[typeid(generic::coreModule::armatureNode).name()] = "[DBHolder]";
#endif
        classList[typeid(spine::SkeletonAnimation).name()] = "[spine]";

        //FairyGUI
//        classList[typeid(fairygui::GBasicTextField).name()] = "[GBasicTextField]";
//        classList[typeid(fairygui::GButton).name()] = "[GButton]";
//        classList[typeid(fairygui::GComboBox).name()] = "[GComboBox]";
//        classList[typeid(fairygui::GComponent).name()] = "[GComponent]";
//        classList[typeid(fairygui::GController).name()] = "[GController]";
//        classList[typeid(fairygui::GGraph).name()] = "[GGraph]";
//        classList[typeid(fairygui::GGroup).name()] = "[GGroup]";
//        classList[typeid(fairygui::GImage).name()] = "[GImage]";
//        classList[typeid(fairygui::GLabel).name()] = "[GLabel]";
//        classList[typeid(fairygui::GList).name()] = "[GList]";
//        classList[typeid(fairygui::GLoader).name()] = "[GLoader]";
//        classList[typeid(fairygui::GMovieClip).name()] = "[GMovieClip]";
//        classList[typeid(fairygui::GObject).name()] = "[GObject]";
//        classList[typeid(fairygui::GObjectPool).name()] = "[GObjectPool]";
//        classList[typeid(fairygui::GProgressBar).name()] = "[GProgressBar]";
//        classList[typeid(fairygui::GRichTextField).name()] = "[GRichTextField]";
//        classList[typeid(fairygui::GRoot).name()] = "[GRoot]";
//        classList[typeid(fairygui::GScrollBar).name()] = "[GScrollBar]";
//        classList[typeid(fairygui::GSlider).name()] = "[GSlider]";
//        classList[typeid(fairygui::GTextField).name()] = "[GTextField]";
//        classList[typeid(fairygui::GTextInput).name()] = "[GTextInput]";
//        classList[typeid(fairygui::GTree).name()] = "[GTree]";
//        classList[typeid(fairygui::GTreeNode).name()] = "[GTreeNode]";
//        classList[typeid(fairygui::GTween).name()] = "[GTween]";
        classList[typeid(fairygui::FUIContainer).name()] = "[FUIContainer]";
        classList[typeid(fairygui::FUIInnerContainer).name()] = "[FUIInnerContainer]";
        classList[typeid(fairygui::FUIInput).name()] = "[FUIInput]";
        classList[typeid(fairygui::FUILabel).name()] = "[FUILabel]";
        classList[typeid(fairygui::FUIRichText).name()] = "[FUIRichText]";
        classList[typeid(fairygui::FUISprite).name()] = "[FUISprite]";
    }

    GET_IMGUI_CONTROLLER().addCallback("mainOverlay", [](){
        auto text_size = ImGui::CalcTextSize("Tools");
        if (default_width == 0.f) {
            default_width = text_size.x + 8.f;
        }
        ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
        ImGui::SetNextWindowContentSize({ default_width, 0 });

        if (!closeAll
            && ImGui::Begin("Tools",
                            &m_enabled,
                            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize
                              | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize)) {
            if (ImGui::Button("Tools")) {
                debugOpened = !debugOpened;
            }
            if (debugOpened) {
                auto temp_size = default_width;
                if (ImGui::Button("Editor")) {
                    nodeEditorOpened = !nodeEditorOpened;
                }
                for (const auto& item : overlayModules) {
                    if (item.second != nullptr) {
                        item.second();
                        auto labelSize = ImGui::CalcTextSize(item.first.c_str());
                        if (labelSize.x + 8.f > default_width)
                            default_width = labelSize.x + 8.f;
                    }
                }
                if (ImGui::Button("Close")) {
                    closeAll = !closeAll;
                }
            } else {
                default_width = 0.f;
            }

            ImGui::End();
        }

        if (nodeEditorOpened)
            showNodeEditor(&nodeEditorOpened);
    });
}

void mainDebugOverlay::addOverlayModule(const std::pair<std::string, std::function<void()>>& item) {
    overlayModules.emplace_back(item);
}

void mainDebugOverlay::removeOverlayModule(const std::string& name) {
    auto find = std::find_if(overlayModules.begin(), overlayModules.end(), [&name](const std::pair<std::string, std::function<void()>>& c){
        return c.first == name;
    });
    if (find != overlayModules.end()) {
        overlayModules.erase(find);
    }
}

void mainDebugOverlay::showNodeEditor(bool* editorOpened) {
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(nodeEditorW), static_cast<float>(nodeEditorH)), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Property editor", reinterpret_cast<bool*>(editorOpened))) {
        ImGui::End();
        return;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
    ImGui::Columns(2, "AB");


    ImGui::BeginChild("left");
    renderTree(ax::Director::getInstance()->getRunningScene()->getChildren());
    ImGui::EndChild();

    ImGui::NextColumn();

    ImGui::BeginChild("right");
    renderPreferences(lastTarget);
    ImGui::EndChild();

    ImGui::PopStyleVar();
    ImGui::End();
}

ImRect mainDebugOverlay::renderTree(const ax::Vector<ax::Node*>& n) {
    const ImRect nodeRect = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
    if (lastTarget == nullptr) {
        lastTarget = n.front();
    }
    for (auto& node : n) {
        ImGui::AlignTextToFramePadding();
        std::string className;
        //		auto test = typeid(*node).name(); // need only for debug
        if (classList.find(typeid(*node).name()) != classList.end()) {
            className = classList[typeid(*node).name()] + " ";
        }
        std::string name = cocos2d::StringUtils::format("%s%s%s", className.c_str(), node->getName().data(), node->isVisible() ? "" : " #inactive");
        if (auto fairy = dynamic_cast<fairygui::FUIContainer*>(node)) {
            if (fairy->gOwner)
                name = cocos2d::StringUtils::format("%s%s[%s]%s", className.c_str(), fairy->gOwner->name.c_str(), fairy->gOwner->id.c_str(), node->isVisible() ? "" : " #inactive");
        }
        ImGuiTreeNodeFlags nodeFlags =
          ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (lastTarget == node) {
            nodeFlags |= ImGuiTreeNodeFlags_Selected;
        }
        if (node->getChildren().empty() || (node->getChildren().size() == 1 && node->getChildren().front()->getName() == "debugNode")) {
            nodeFlags |= ImGuiTreeNodeFlags_Leaf;
        }
        bool isNodeProperties = false;
        if (dynamic_cast<generic::coreModule::nodeProperties*>(node)) {
            isNodeProperties = true;
        }
        if (isNodeProperties) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.22f, 0.75f, 0.125f, 1.f));
        }
        bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)node->_ID, nodeFlags, "%s", name.c_str());
        if (isNodeProperties) {
            ImGui::PopStyleColor(1);
        }
        if (ImGui::IsItemClicked()) {
            // id of clicked element
            lastTarget = node;
        }
        if (nodeOpen) {
            ImGui::PushID(node);

            renderTree(node->getChildren());
            ImGui::PopID();
            ImGui::TreePop();
        }
    }


    return nodeRect;
}

ImRect mainDebugOverlay::renderPreferences(ax::Node* node) {
    const ImRect prefRect = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
    auto tempNode = static_cast<ax::Ref*>(node);
    if (node == nullptr || tempNode->getReferenceCount() == 0u) {
        lastTarget = nullptr;
        return prefRect;
    }
    ImGui::PushID(node->getName().data());

    if (_propertyViewer) {
        bool fairyFind = false;
        if (auto fairy = dynamic_cast<fairygui::FUIContainer*>(node)) {
            if (fairy->gOwner) {
                _propertyViewer->updateFairy(fairy->gOwner);
                fairyFind = true;
            }
        }
        if (!fairyFind) {
            _propertyViewer->update(node);
        }
    }

    ImGui::PopID();

    return prefRect;
}
