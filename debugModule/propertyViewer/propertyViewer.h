#pragma once
#ifndef GENERIC_PROPERTYVIEWER_H
#define GENERIC_PROPERTYVIEWER_H

#include <vector>

//forward
namespace ax {
    class Node;
}
namespace fairygui {
    class GObject;
}

namespace generic::debugModule {

    // forward
    class propertyInterface;

    class propertyViewer {
    public:
        propertyViewer();
        ~propertyViewer();
        void update(ax::Node* node);
        void updateFairy(fairygui::GObject* node);

    private:
        std::vector<propertyInterface*> _propertiesList;
    };
}// namespace generic::debugModule

#endif// GENERIC_PROPERTYVIEWER_H
