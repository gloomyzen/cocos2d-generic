#pragma once
#ifndef GENERIC_INITDEBUG_H
#define GENERIC_INITDEBUG_H

#include "axmol.h"
#include "ImGui/imgui/imgui_internal.h"
#include <vector>
#include <string>
#include <tuple>
#include <functional>

namespace generic::debugModule {

    //forward
    class propertyViewer;

    class mainDebugOverlay {
    public:
        static void init();

        static void addOverlayModule(const std::pair<std::string, std::function<void()>>&);
        static void removeOverlayModule(const std::string&);
    private:
        static void showNodeEditor(bool* editorOpened);
        static ImRect renderTree(const ax::Vector<ax::Node*>& n);
        static ImRect renderPreferences(ax::Node*);

        static bool m_enabled;
        static float default_width;
        /***
         * Debug buttonNode properties
         */
        static int nodeEditorW;
        static int nodeEditorH;
        static ax::Node* lastTarget;

        static propertyViewer* _propertyViewer;
        static std::vector<std::pair<std::string, std::function<void()>>> overlayModules;
        static std::map<std::string, std::string> classList;
    };
}


#endif// GENERIC_INITDEBUG_H
