#pragma once
#ifndef IMGUI_MODULE_IMGUICONTROLLER_H
#define IMGUI_MODULE_IMGUICONTROLLER_H
#include "axmol.h"
#include "ImGui/ImGuiPresenter.h"
#include <vector>
#include <functional>
#include <unordered_map>
#include <string>
#include <tuple>
#include "generic/coreModule/signals/signalHolder.h"

#define GET_IMGUI_CONTROLLER() generic::debugModule::imguiController::getInstance()

namespace generic::debugModule {

    struct sImguiModuleSignals {
        generic::signal::signalHolder<> onSceneChanging;
        generic::signal::signalHolder<> onImGuiLayerInit;
        generic::signal::signalHolder<> onNewFrame;
    };

    class imguiController {
    public:
        ~imguiController();
        static imguiController& getInstance();
        static void cleanup();

        sImguiModuleSignals& getEmitter();

        void draw();
        void addCallback(const std::string& name, const std::function<void()>& callBack);
        void removeCallback(const std::string& name);

        // imgui helper
        static void dragFloat(const std::string& label, const std::vector<float>& vals, int vSpeed, const std::function<void(const std::vector<float>&)>& changeClb);
        static void dragFloat(const std::string& label, const ax::Size& vals, int vSpeed, const std::function<void(const ax::Size&)>& changeClb);
        static bool
          Combo(const char* label, int* current_item, const std::vector<std::string>& items, int items_count, int height_in_items = -1);
        void image(ax::Texture2D* tex,
                   const ImVec2& size,
                   const ImVec2& uv0 = ImVec2(0, 0),
                   const ImVec2& uv1 = ImVec2(1, 1),
                   const ImVec4& tint_col = ImVec4(1, 1, 1, 1),
                   const ImVec4& border_col = ImVec4(0, 0, 0, 0));
        void image(ax::Sprite* sprite,
                   const ImVec2& size,
                   const ImVec4& tint_col = ImVec4(1, 1, 1, 1),
                   const ImVec4& border_col = ImVec4(0, 0, 0, 0));
        bool imageButton(ax::Texture2D* tex,
                         const ImVec2& size,
                         const ImVec2& uv0 = ImVec2(0, 0),
                         const ImVec2& uv1 = ImVec2(1, 1),
                         int frame_padding = -1,
                         const ImVec4& bg_col = ImVec4(0, 0, 0, 0),
                         const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
        bool imageButton(ax::Sprite* sprite,
                         const ImVec2& size,
                         int frame_padding = -1,
                         const ImVec4& bg_col = ImVec4(0, 0, 0, 0),
                         const ImVec4& tint_col = ImVec4(1, 1, 1, 1));

        void node(ax::Node* node, const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));
        bool nodeButton(ax::Node* node,
                        int frame_padding = -1,
                        const ImVec4& bg_col = ImVec4(0, 0, 0, 0),
                        const ImVec4& tint_col = ImVec4(1, 1, 1, 1));

        std::tuple<ImTextureID, int> useTexture(ax::Texture2D* texture);
        std::tuple<ImTextureID, ImVec2, ImVec2, int> useSprite(ax::Sprite* sprite);
        std::tuple<ImTextureID, ImVec2, ImVec2, int> useNode(ax::Node* node, const ImVec2& pos);

        static void setNodeColor(ax::Node* node, const ImVec4& col);
        static void setNodeColor(ax::Node* node, ImGuiCol col);
        static void setLabelColor(ax::Label* label, const ImVec4& col);
        static void setLabelColor(ax::Label* label, bool disabled = false);
        static void setLabelColor(ax::Label* label, ImGuiCol col);

        ImWchar* addGlyphRanges(const std::string& key, const std::vector<ImWchar>& ranges);
        static void mergeFontGlyphs(ImFont* dst, ImFont* src, ImWchar start, ImWchar end);
        int getCCRefId(ax::Ref* p);

    private:
        imguiController() = default;
        imguiController(const imguiController&) = default;
        imguiController& operator=(const imguiController&) = default;
        static void create();
        static void onDeadReference();

        static imguiController* pInstance;
        static bool destroyed;
        sImguiModuleSignals emitter;

        std::unordered_map<std::string, std::function<void()>> _callPiplines;
        std::unordered_map<ax::Ref*, int> usedCCRefIdMap;
        // cocos objects should be retained until next frame
        ax::Vector<ax::Ref*> usedCCRef;
        std::unordered_map<std::string, std::vector<ImWchar>> glyphRanges;
    };
}// namespace debugModule

#endif// IMGUI_MODULE_IMGUICONTROLLER_H
