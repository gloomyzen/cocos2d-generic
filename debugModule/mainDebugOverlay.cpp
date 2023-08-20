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
std::map<std::type_index, std::string> mainDebugOverlay::classList = {};

static bool closeAll = false;
static bool debugOpened = false;
static bool nodeEditorOpened = false;

void mainDebugOverlay::init() {
    if (_propertyViewer)
        return;
    _propertyViewer = new propertyViewer();

    {
        // General
        classList[typeid(ax::Camera)] = "[Camera]";
        classList[typeid(ax::Scene)] = "[Scene]";
        // Nodes
        classList[typeid(ax::Node)] = "[Node]";
        classList[typeid(ax::ClippingNode)] = "[ClippingNode]";
        classList[typeid(ax::DrawNode)] = "[DrawNode]";
        classList[typeid(ax::ParticleSystem)] = "[ParticleSystem]";
        classList[typeid(ax::ParticleBatchNode)] = "[ParticleBatchNode]";
        classList[typeid(ax::SpriteBatchNode)] = "[SpriteBatchNode]";
        classList[typeid(ax::ParallaxNode)] = "[ParallaxNode]";
        classList[typeid(ax::RenderTexture)] = "[RenderTexture]";
        classList[typeid(ax::ClippingRectangleNode)] = "[ClippingRectangleNode]";
        classList[typeid(ax::AttachNode)] = "[AttachNode]";
        classList[typeid(ax::Sprite)] = "[Sprite]";
        classList[typeid(ax::NodeGrid)] = "[NodeGrid]";
        classList[typeid(ax::Label)] = "[Label]";
        classList[typeid(ax::Menu)] = "[Menu]";
        classList[typeid(ax::MenuItem)] = "[MenuItem]";
        // todo add other menu types

        // Widgets
        classList[typeid(ax::ui::Widget)] = "[ui::Widget]";
        classList[typeid(ax::ui::Layout)] = "[ui::Layout]";
        classList[typeid(ax::ui::Button)] = "[ui::Button]";
        classList[typeid(ax::ui::ListView)] = "[ui::ListView]";
        classList[typeid(ax::ui::AbstractCheckButton)] = "[ui::AbstractCheckButton]";
        classList[typeid(ax::ui::CheckBox)] = "[ui::CheckBox]";
        classList[typeid(ax::ui::RadioButton)] = "[ui::RadioButton]";
        classList[typeid(ax::ui::RadioButtonGroup)] = "[ui::RadioButtonGroup]";
        classList[typeid(ax::ui::PageView)] = "[ui::PageView]";
        classList[typeid(ax::ui::ImageView)] = "[ui::ImageView]";
        classList[typeid(ax::ui::ScrollView)] = "[ui::ScrollView]";
        classList[typeid(ax::ui::Slider)] = "[ui::Slider]";
        classList[typeid(ax::ui::EditBox)] = "[ui::EditBox]";
        classList[typeid(ax::ui::LoadingBar)] = "[ui::LoadingBar]";
        classList[typeid(ax::ui::Text)] = "[ui::Text]";
        classList[typeid(ax::ui::TextField)] = "[ui::TextField]";
        classList[typeid(ax::ui::RichText)] = "[ui::RichText]";
        classList[typeid(ax::ui::TabControl)] = "[ui::TabControl]";
        classList[typeid(ax::ui::TabHeader)] = "[ui::TabHeader]";
        // 3D & Light
        classList[typeid(ax::Sprite3D)] = "[Sprite3D]";
        classList[typeid(ax::BillBoard)] = "[BillBoard]";
        classList[typeid(ax::Skybox)] = "[Skybox]";
        classList[typeid(ax::Mesh)] = "[Mesh]";
        classList[typeid(ax::Terrain)] = "[Terrain]";
        classList[typeid(ax::AmbientLight)] = "[AmbientLight]";
        classList[typeid(ax::BaseLight)] = "[BaseLight]";
        classList[typeid(ax::DirectionLight)] = "[DirectionLight]";
        classList[typeid(ax::PointLight)] = "[PointLight]";
        classList[typeid(ax::SpotLight)] = "[SpotLight]";
        // Layers
        classList[typeid(ax::Layer)] = "[Layer]";
        classList[typeid(ax::LayerColor)] = "[LayerColor]";
        // Actions
        classList[typeid(ax::Action)] = "[Action]";
        // DragonBones node
#if defined(AX_BUILD_WITH_DRANGBONES) && AX_BUILD_WITH_DRANGBONES
        classList[typeid(dragonBones::CCArmatureDisplay)] = "[DBArmature]";
        classList[typeid(generic::coreModule::armatureNode)] = "[DBHolder]";
#endif
        classList[typeid(spine::SkeletonAnimation)] = "[spine]";

        //FairyGUI
//        classList[typeid(fairygui::GBasicTextField)] = "[GBasicTextField]";
//        classList[typeid(fairygui::GButton)] = "[GButton]";
//        classList[typeid(fairygui::GComboBox)] = "[GComboBox]";
//        classList[typeid(fairygui::GComponent)] = "[GComponent]";
//        classList[typeid(fairygui::GController)] = "[GController]";
//        classList[typeid(fairygui::GGraph)] = "[GGraph]";
//        classList[typeid(fairygui::GGroup)] = "[GGroup]";
//        classList[typeid(fairygui::GImage)] = "[GImage]";
//        classList[typeid(fairygui::GLabel)] = "[GLabel]";
//        classList[typeid(fairygui::GList)] = "[GList]";
//        classList[typeid(fairygui::GLoader)] = "[GLoader]";
//        classList[typeid(fairygui::GMovieClip)] = "[GMovieClip]";
//        classList[typeid(fairygui::GObject)] = "[GObject]";
//        classList[typeid(fairygui::GObjectPool)] = "[GObjectPool]";
//        classList[typeid(fairygui::GProgressBar)] = "[GProgressBar]";
//        classList[typeid(fairygui::GRichTextField)] = "[GRichTextField]";
//        classList[typeid(fairygui::GRoot)] = "[GRoot]";
//        classList[typeid(fairygui::GScrollBar)] = "[GScrollBar]";
//        classList[typeid(fairygui::GSlider)] = "[GSlider]";
//        classList[typeid(fairygui::GTextField)] = "[GTextField]";
//        classList[typeid(fairygui::GTextInput)] = "[GTextInput]";
//        classList[typeid(fairygui::GTree)] = "[GTree]";
//        classList[typeid(fairygui::GTreeNode)] = "[GTreeNode]";
//        classList[typeid(fairygui::GTween)] = "[GTween]";
        classList[typeid(fairygui::FUIContainer)] = "[FUIContainer]";
        classList[typeid(fairygui::FUIInnerContainer)] = "[FUIInnerContainer]";
        classList[typeid(fairygui::FUIInput)] = "[FUIInput]";
        classList[typeid(fairygui::FUILabel)] = "[FUILabel]";
        classList[typeid(fairygui::FUIRichText)] = "[FUIRichText]";
        classList[typeid(fairygui::FUISprite)] = "[FUISprite]";
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
        if (classList.count(typeid(*node)) > 0U) {
            className = classList[typeid(*node)];
        }
        std::string name = cocos2d::StringUtils::format("%s %s%s", className.c_str(), node->getName().data(), node->isVisible() ? "" : " #inactive");
        if (auto fairy = dynamic_cast<fairygui::FUIContainer*>(node)) {
            if (fairy->gOwner)
                name = cocos2d::StringUtils::format("%s[id:%s] %s%s", className.c_str(), fairy->gOwner->id.c_str(), fairy->gOwner->name.c_str(), node->isVisible() ? "" : " #inactive");
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
