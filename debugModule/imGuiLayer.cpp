#ifdef DEBUG
#include "imGuiLayer.h"
#ifdef ADXE_VERSION
#include "ImGuiEXT/imgui/misc/cpp/imgui_stdlib.h"
#else
#include "imgui/misc/cpp/imgui_stdlib.h"
#endif
#include "generic/coreModule/nodes/nodeProperties.h"
#include "generic/utilityModule/stringUtility.h"
#include "spine/spine-cocos2dx.h"

using namespace generic;
using namespace generic::debugModule;
using namespace generic::coreModule;
using namespace cocos2d;

static bool closeAll = false;
static bool debugOpened = false;
static bool nodeEditorOpened = false;

bool imGuiLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    {
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
        // DragonBones node
#if defined(CC_BUILD_WITH_DRANGBONES) && CC_BUILD_WITH_DRANGBONES
        classList[typeid(dragonBones::CCArmatureDisplay).name()] = "[DBArmature]";
        classList[typeid(generic::coreModule::armatureNode).name()] = "[DBHolder]";
#endif
        classList[typeid(spine::SkeletonAnimation).name()] = "[spine]";
    }

    setName("ImGUILayer");
    scheduleUpdate();
#ifdef ADXE_VERSION
    // Resize (expand) window
    auto director = Director::getInstance();
    auto view = dynamic_cast<GLViewImpl*>(Director::getInstance()->getOpenGLView());
    view->setWindowed(director->getVisibleSize().width, director->getVisibleSize().height);
    cocos2d::extension::ImGuiEXT::getInstance()->addFont(FileUtils::getInstance()->fullPathForFilename("fonts/arial.ttf"));
    cocos2d::extension::ImGuiEXT::getInstance()->addRenderLoop(
      "#test", CC_CALLBACK_0(imGuiLayer::drawImgui, this), director->getRunningScene());
#else
    auto layer = ImGuiLayer::create();
    addChild(layer);
    CCIMGUI::getInstance()->addCallback([this](){
        drawImgui();
    }, "buttons");
#endif
    initEvents();

    return true;
}

void imGuiLayer::drawImgui() {
    // Buttons
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
            for (const auto& item : debugModules) {
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
}

void imGuiLayer::showNodeEditor(bool* editorOpened) {
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(nodeEditorW), static_cast<float>(nodeEditorH)), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Property editor", reinterpret_cast<bool*>(editorOpened))) {
        ImGui::End();
        return;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
    ImGui::Columns(2, "AB");


    ImGui::BeginChild("left");
    renderTree(cocos2d::Director::getInstance()->getRunningScene()->getChildren());
    ImGui::EndChild();

    ImGui::NextColumn();

    ImGui::BeginChild("right");
    renderPreferences(lastTarget);
    ImGui::EndChild();

    ImGui::PopStyleVar();
    ImGui::End();
}

ImRect imGuiLayer::renderTree(cocos2d::Vector<Node*> n) {
    const ImRect nodeRect = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
    if (lastTarget == nullptr) {
        lastTarget = n.front();
    }
    for (auto& node : n) {
        if (node->getName() == "debugNode")
            continue;
        if (typeid(*node).name() == typeid(imGuiLayer).name())
            continue;
        ImGui::AlignTextToFramePadding();
        std::string className;
        //		auto test = typeid(*node).name(); // need only for debug
        if (classList.find(typeid(*node).name()) != classList.end()) {
            className = classList[typeid(*node).name()] + " ";
        }
        const std::string name = className + node->getName() + (node->isVisible() ? "" : " #inactive");
        ImGuiTreeNodeFlags nodeFlags =
          ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (lastTarget == node) {
            nodeFlags |= ImGuiTreeNodeFlags_Selected;
        }
        if (node->getChildren().empty() || (node->getChildren().size() == 1 && node->getChildren().front()->getName() == "debugNode")) {
            nodeFlags |= ImGuiTreeNodeFlags_Leaf;
        }
        bool isNodeProperties = false;
        if (dynamic_cast<nodeProperties*>(node)) {
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

ImRect imGuiLayer::renderPreferences(Node* node) {
    const ImRect prefRect = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
    auto tempNode = static_cast<Ref*>(node);
    if (node == nullptr || tempNode->getReferenceCount() == 0u) {
        lastTarget = nullptr;
        return prefRect;
    }
    ImGui::PushID(node->getName().c_str());

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader("General info")) {
        ImGui::Text("Node Name(ID) %s", node->getName().c_str());
        ImGui::Text("Node GUI %d", node->_ID);
        if (node->isRunning()) {
            auto active = node->isVisible();
            auto tempActive = active;
            ImGui::Checkbox(" Is active", &active);
            if (active != tempActive && node->getName() != "ImGUILayer") {
                node->setVisible(active);
            }
        }
    }

    debugToggleRow(node);

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader("Transform")) {
        /**
         * Height and Width
         */
        auto width = node->getContentSize().width;
        auto height = node->getContentSize().height;
        float vecWH[2] = { width, height };
        ImGui::DragFloat2("Width/Height", vecWH, 1);
        if (vecWH[0] != width || vecWH[1] != height) {
            auto _size = cocos2d::Size();
            _size.width = vecWH[0];
            _size.height = vecWH[1];
            node->setContentSize(_size);
        }
        /**
         * Position by X Y Z
         */
        auto nodePos = node->getPosition3D();
        float vecPos[3] = { nodePos.x, nodePos.y, nodePos.z };
        ImGui::DragFloat3("Position X/Y", vecPos, 1.f);
        if (vecPos[0] != nodePos.x || vecPos[1] != nodePos.y || vecPos[2] != nodePos.z) {
            cocos2d::Vec3 vec3 = { vecPos[0], vecPos[1], vecPos[2] };
            node->setPosition3D(vec3);
        }
        /**
         * Pivot
         */
        auto nodePivot = node->getPivotPoint();
        float vecPivot[2] = { nodePivot.x, nodePivot.y };
        ImGui::DragFloat2("Pivot X/Y", vecPivot, 0.01f, -10.f, 10.f);
        if (vecPivot[0] != nodePivot.x || vecPivot[1] != nodePivot.y) {
            node->setPivotPoint({ vecPivot[0], vecPivot[1] });
        }
        /**
         * Anchor
         */
        auto nodeAnchor = node->getAnchorPoint();
        float vecAnchor[2] = { nodeAnchor.x, nodeAnchor.y };
        ImGui::DragFloat2("Anchor X/Y", vecAnchor, 0.01f, -100.f, 100.f);
        if (vecAnchor[0] != nodeAnchor.x || vecAnchor[1] != nodeAnchor.y) {
            node->setAnchorPoint({ vecAnchor[0], vecAnchor[1] });
        }
        /**
         * Scale
         */
        auto nodeScaleX = node->getScaleX();
        auto nodeScaleY = node->getScaleY();
        auto tempScaleX = nodeScaleX;
        auto tempScaleY = nodeScaleY;
        float vecScale[2] = { nodeScaleX, nodeScaleY };
        ImGui::DragFloat2("Scale X/Y", vecScale, 0.01f);
        if (tempScaleX != vecScale[0] || tempScaleY != vecScale[1]) {
            node->setScaleX(vecScale[0]);
            node->setScaleY(vecScale[1]);
        }
        /**
         * Skew X and Y
         */
        auto nodeSkewX = node->getSkewX();
        auto nodeSkewY = node->getSkewY();
        auto tempSkewX = nodeSkewX;
        auto tempSkewY = nodeSkewY;
        float vecSkew[2] = { nodeSkewX, nodeSkewY };
        ImGui::DragFloat2("Skew X/Y", vecSkew, 0.1f);
        if (tempSkewX != vecSkew[0] || tempSkewY != vecSkew[1]) {
            node->setSkewX(vecSkew[0]);
            node->setSkewY(vecSkew[1]);
        }
        /**
         * Rotation
         */
        auto nodeRotation = node->getRotation();
        auto tempRotation = nodeRotation;
        float floatRotation[1] = { nodeRotation };
        ImGui::DragFloat("Rotation", floatRotation, 0.1f);
        if (floatRotation[0] != tempRotation) {
            node->setRotation(floatRotation[0]);
        }
        /**
         * Set cascade opacity
         */
        auto isCascadeOpacity = node->isCascadeOpacityEnabled();
        auto tempCascadeOpacity = isCascadeOpacity;
        ImGui::Checkbox("Cascade opacity", &tempCascadeOpacity);
        if (tempCascadeOpacity != isCascadeOpacity) {
            node->setCascadeOpacityEnabled(tempCascadeOpacity);
        }
        /**
         * Set opacity
         */
        auto nodeOpacity = node->getOpacity();
        auto tempOpacity = nodeOpacity;
        int changedOpacity = static_cast<int>(nodeOpacity);
        ImGui::DragInt("Opacity", &changedOpacity, 1, 0, 255);
        if (changedOpacity != tempOpacity) {
            node->setOpacity(static_cast<uint8_t>(changedOpacity));
        }
    }
    if (ImGui::CollapsingHeader("Color")) {
        /**
         * Set cascade opacity
         */
        auto isCascadeColor = node->isCascadeColorEnabled();
        auto tempCascadeColor = isCascadeColor;
        ImGui::Checkbox("Cascade color", &tempCascadeColor);
        if (tempCascadeColor != isCascadeColor) {
            node->setCascadeColorEnabled(tempCascadeColor);
        }
        /***
         * Sprite Color
         */
        auto nodeColor = node->getColor();
        float nodeRed = nodeColor.r / 255.0f;
        float nodeGreen = nodeColor.g / 255.0f;
        float nodeBlue = nodeColor.b / 255.0f;
        float nodeAlpha = node->getOpacity() / 255.0f;
        float vecColors[4] = { nodeRed, nodeGreen, nodeBlue, nodeAlpha };
        ImGui::ColorEdit4(
          "Color", (float*)&vecColors, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Uint8);
        if (vecColors[0] != static_cast<float>(nodeColor.r) / 255.0f) {
            nodeColor.r = static_cast<uint8_t>(vecColors[0] * 255.0f);
            if (nodeColor.r < 0)
                nodeColor.r = 0;
            else if (nodeColor.r >= 255)
                nodeColor.r = 255;
            node->setColor(nodeColor);
        }
        if (vecColors[1] != static_cast<float>(nodeColor.g) / 255.0f) {
            nodeColor.g = static_cast<uint8_t>(vecColors[1] * 255.0f);
            if (nodeColor.g < 0)
                nodeColor.g = 0;
            else if (nodeColor.g >= 255)
                nodeColor.g = 255;
            node->setColor(nodeColor);
        }
        if (vecColors[2] != static_cast<float>(nodeColor.b) / 255.0f) {
            nodeColor.b = static_cast<uint8_t>(vecColors[2] * 255.0f);
            if (nodeColor.b < 0)
                nodeColor.b = 0;
            else if (nodeColor.b >= 255)
                nodeColor.b = 255;
            node->setColor(nodeColor);
        }
        if (vecColors[3] != static_cast<float>(nodeAlpha) / 255.0f) {
            nodeAlpha = static_cast<uint8_t>(vecColors[3] * 255.0f);
            if (nodeAlpha < 0)
                nodeAlpha = 0;
            else if (nodeAlpha >= 255)
                nodeAlpha = 255;
            node->setOpacity(static_cast<uint8_t>(nodeAlpha));
        }
    }
    if (auto light = dynamic_cast<cocos2d::DirectionLight*>(node)) {
        if (ImGui::CollapsingHeader("DirectionLight node")) {
            auto local = light->getDirection();
            float changedLocal[3] = { local.x, local.y, local.z };
            ImGui::DragFloat3("Direction", changedLocal, .01f, -1, 1);
            if (changedLocal[0] != local.x || changedLocal[1] != local.y || changedLocal[2] != local.y) {
                light->setDirection(cocos2d::Vec3(changedLocal[0], changedLocal[1], changedLocal[2]));
            }
        }
    }
    if (ImGui::CollapsingHeader("Z-Order")) {
        auto local = node->getLocalZOrder();
        int changedLocal = local;
        ImGui::DragInt("Local z-order", &changedLocal, 1, -999, 999);
        if (changedLocal != local) {
            node->setLocalZOrder(static_cast<std::int32_t>(changedLocal));
        }
    }
    if (auto spriteNode = dynamic_cast<cocos2d::Sprite*>(node)) {
        if (ImGui::CollapsingHeader("Sprite")) {
            ImGui::Text("Image path \"%s\"", spriteNode->getResourceName().c_str());
        }
    }
#if defined(CC_BUILD_WITH_DRANGBONES) && CC_BUILD_WITH_DRANGBONES
    if (auto dragonbonesNode = dynamic_cast<generic::coreModule::armatureNode*>(node)) {
        if (ImGui::CollapsingHeader("DragonBones node")) {
            // animation sections
            if (auto armature = dragonbonesNode->getArmatureNode()) {
                auto animNames = armature->getAnimation()->getAnimationNames();
                auto lastAnimation = armature->getAnimation()->getLastAnimationName();
                int itemCurrent = 1;
                std::vector<std::string> items;
                items.reserve(animNames.size());
                for (int i = 0; i < animNames.size(); ++i) {
                    items.emplace_back(animNames[i]);
                    if (lastAnimation == animNames[i]) {
                        itemCurrent = i;
                    }
                }
                int tempItem = itemCurrent;
                Combo("Animation", &itemCurrent, items, static_cast<int>(items.size()));
                if (tempItem != itemCurrent) {
                    //                    auto cfg = new dragonBones::AnimationConfig();
                    //                    cfg->actionEnabled = true;
                    //                    cfg->animation = items[itemCurrent];
                    //                    armature->getAnimation()->playConfig(cfg);
                    //                    armature->getAnimation()->play(items[itemCurrent]);
                    auto state = armature->getAnimation()->fadeIn(animNames[itemCurrent]);
                }
            }
        }
    }
#endif
    if (auto spine = dynamic_cast<spine::SkeletonAnimation*>(node)) {
        if (ImGui::CollapsingHeader("Spine node")) {
            if (auto skeleton = spine->getSkeleton()) {
                if (auto data = skeleton->getData()) {
                    {
                        // a new
                        auto isLoop = false;
                        auto animations = data->getAnimations();
                        std::string lastAnimation;
                        if (auto track = spine->getState()->getCurrent(1)) {
                            lastAnimation = track->getAnimation()->getName().buffer();
                            isLoop = track->getLoop();
                            auto tempLoop = isLoop;
                            ImGui::Checkbox("Loop animation", &isLoop);
                            if (tempLoop != isLoop) {
                                track->setLoop(isLoop);
                            }
                        }
                        int itemCurrent = 1;
                        std::vector<std::string> items;
                        items.reserve(animations.size());
                        for (auto i = 0; i < animations.size(); ++i) {
                            items.emplace_back(animations[i]->getName().buffer());
                            if (lastAnimation == animations[i]->getName().buffer()) {
                                itemCurrent = i;
                            }
                        }

                        int tempItem = itemCurrent;
                        Combo("Animation", &itemCurrent, items, static_cast<int>(items.size()));
                        if (tempItem != itemCurrent) {
                            spine->setAnimation(1, std::string(animations[itemCurrent]->getName().buffer()), isLoop);
                        }
                    }

                    {
                        // skins section
                        auto skins = data->getSkins();
                        int itemCurrent = 1;
                        std::vector<std::string> items;
                        items.reserve(skins.size());
                        for (auto i = 0; i < skins.size(); ++i) {
                            items.emplace_back(skins[i]->getName().buffer());
                        }
                        Combo("Skins", &itemCurrent, items, static_cast<int>(items.size()));
                    }
                }
                {
                    // skeleton scale section
                    float skeletonScale[2] = { skeleton->getScaleX(), skeleton->getScaleY() };
                    ImGui::DragFloat2("Skeleton scale", skeletonScale, .01f, -1, 1);
                    if (skeletonScale[0] != skeleton->getScaleX() || skeletonScale[1] != skeleton->getScaleY()) {
                        skeleton->setScaleX(skeletonScale[0]);
                        skeleton->setScaleY(skeletonScale[1]);
                    }
                }
                {
                    // skeleton slots section
                    auto slots = skeleton->getSlots();
                    int itemCurrent = 1;
                    std::vector<std::string> items;
                    items.reserve(slots.size());
                    for (auto i = 0; i < slots.size(); ++i) {
                        items.emplace_back(slots[i]->getData().getName().buffer());
                    }
                    Combo("Slots", &itemCurrent, items, static_cast<int>(items.size()));
                }
            }
            {
                // debug bones section
                auto debugBones = spine->getDebugBonesEnabled();
                auto tempDebugBones = debugBones;
                ImGui::Checkbox("Bones debug", &debugBones);
                if (tempDebugBones != debugBones) {
                    spine->setDebugBonesEnabled(debugBones);
                }
            }
            {
                // debug bounding rect section
                auto debugBones = spine->getDebugBoundingRectEnabled();
                auto tempDebugBones = debugBones;
                ImGui::Checkbox("BoundingRect debug", &debugBones);
                if (tempDebugBones != debugBones) {
                    spine->setDebugBoundingRectEnabled(debugBones);
                }
            }
            {
                // debug Meshes section
                auto debugBones = spine->getDebugMeshesEnabled();
                auto tempDebugBones = debugBones;
                ImGui::Checkbox("Meshes debug", &debugBones);
                if (tempDebugBones != debugBones) {
                    spine->setDebugMeshesEnabled(debugBones);
                }
            }
            {
                // debug Slots section
                auto debugBones = spine->getDebugSlotsEnabled();
                auto tempDebugBones = debugBones;
                ImGui::Checkbox("Slots debug", &debugBones);
                if (tempDebugBones != debugBones) {
                    spine->setDebugSlotsEnabled(debugBones);
                }
            }
        }
    }
    if (auto labelNode = dynamic_cast<cocos2d::Label*>(node)) {
        if (ImGui::CollapsingHeader("Label node")) {
            // todo lh ls color text font shadow
            /*** Text */
            auto labelText = labelNode->getString();
            char* newLabelText = const_cast<char*>(labelText.c_str());
            ImGui::InputText("Text", newLabelText, IM_ARRAYSIZE(newLabelText) * 16);
            if (labelText != newLabelText) {
                labelNode->setString(newLabelText);
            }
            auto labelWidth = labelNode->getMaxLineWidth();
            auto tempWidth = labelWidth;
            float changedWidth = labelWidth;
            ImGui::DragFloat("Max Line Width", &changedWidth, 1.f, 0.f, 9999.f);
            if (changedWidth != tempWidth) {
                labelNode->setMaxLineWidth(changedWidth);
            }
            /*** Label Effects todo not working! */
            /*int labelEffectsIdx = 0;
            auto labelEffect = labelNode->getLabelEffectType();
            for (int i = 0; i < labelEffectList.size(); ++i) {
                if (labelEffectList[i].second == labelEffect) {
                    labelEffectsIdx = i;
                }
            }
            int currentEffectsIdx = labelEffectsIdx;
            std::string elementsList;
            for (const auto &item : labelEffectList) {
                elementsList += item.first + '\0';
            }
            if (ImGui::Combo("Style", &labelEffectsIdx, elementsList.c_str())) {
                if (labelEffectsIdx != currentEffectsIdx) {
                    //
                }
            }*/
        }
    }

    ImGui::PopID();

    return prefRect;
}

void imGuiLayer::debugToggleRow(cocos2d::Node* node) {
    if (node->isRunning()) {
        auto active = node->getDebug();
        auto tempActive = active;
        ImGui::Checkbox("Debug", &active);
        if (active != tempActive) {
            node->setDebug(active);
        }
        /***
         * Debug line colors
         */
        auto nodeColor = node->getDebugLineColor();
        float nodeRed = nodeColor.r;
        float nodeGreen = nodeColor.g;
        float nodeBlue = nodeColor.b;
        float nodeAlpha = nodeColor.a;
        float vecColors[4] = { nodeRed, nodeGreen, nodeBlue, nodeAlpha };
        bool colorChanged = false;
        ImGui::ColorEdit4(
          "Debug color", (float*)&vecColors, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Uint8);
        if (vecColors[0] != nodeColor.r) {
            nodeColor.r = vecColors[0];
            colorChanged = true;
        }
        if (vecColors[1] != nodeColor.g) {
            nodeColor.g = vecColors[1];
            colorChanged = true;
        }
        if (vecColors[2] != nodeColor.b) {
            nodeColor.b = vecColors[2];
            colorChanged = true;
        }
        if (vecColors[3] != nodeColor.a) {
            nodeColor.a = vecColors[3];
            colorChanged = true;
        }
        if (colorChanged) {
            node->setDebugLineColor(nodeColor);
        }
    }
}

void imGuiLayer::resetDebugModules() {
    debugModules.clear();
}

void imGuiLayer::addDebugModules(const std::pair<std::string, std::function<void()>>& item) {
    debugModules.push_back(item);
}

void imGuiLayer::initEvents() {
    keyboardListener = cocos2d::EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = [](cocos2d::EventKeyboard::KeyCode core, cocos2d::Event*) {
        if (core == cocos2d::EventKeyboard::KeyCode::KEY_CTRL) {
            nodeEditorOpened = !nodeEditorOpened;
        }
    };
    keyboardListener->onKeyReleased = [](cocos2d::EventKeyboard::KeyCode core, cocos2d::Event*) {};
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

bool imGuiLayer::Combo(const char* label, int* current_item, const std::vector<std::string>& items, int items_count, int height_in_items) {
    return ImGui::Combo(label, current_item,[](void* data, int idx, const char** out_text) {
          auto strVec = static_cast<const std::vector<std::string>*>(data);
          *out_text = (*strVec)[idx].c_str();
          return true;
      }, (void*)&items, items_count, height_in_items);
}

#endif// DEBUG
