#include "spineProperty.h"
#include "generic/debugModule/imguiController.h"
#include "ImGui/imgui/imgui_internal.h"
#include "axmol.h"
#include "spine/spine-cocos2dx.h"

using namespace generic::debugModule;

spineProperty::~spineProperty() {}

void spineProperty::viewProperty(ax::Node* node) {
    if (auto spine = dynamic_cast<spine::SkeletonAnimation*>(node)) {
        ImGui::SetNextItemOpen(false, ImGuiCond_Once);
        if (ImGui::CollapsingHeader(_propertyType.c_str())) {
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
                        GET_IMGUI_CONTROLLER().Combo("Animation", &itemCurrent, items, static_cast<int>(items.size()));
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
                        GET_IMGUI_CONTROLLER().Combo("Skins", &itemCurrent, items, static_cast<int>(items.size()));
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
                    GET_IMGUI_CONTROLLER().Combo("Slots", &itemCurrent, items, static_cast<int>(items.size()));
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
}

void spineProperty::viewPropertyFairyGUI(fairygui::GObject*) {
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader(_propertyType.c_str())) {
        //
    }
}