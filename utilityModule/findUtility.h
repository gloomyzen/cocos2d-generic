#pragma once
#ifndef GENERIC_FINDUTILITY_H
#define GENERIC_FINDUTILITY_H

#include "randomUtility.h"
#include "axmol.h"
#include "fairygui/FairyGUI.h"
#include <tuple>
#include <vector>
#include <string>
#include <string_view>

namespace generic::utilityModule {

    static ax::Node* findNode(ax::Node* node, const std::string_view& name) {
        if (!node || name.empty())
            return nullptr;
        if (node->getName() == name) {
            return node;
        }
        ax::Node* nodeFound = node->getChildByName(name);
        if (!nodeFound) {
            auto children = node->getChildren();
            for (auto child : children) {
                nodeFound = findNode(child, name);
                if (nodeFound)
                    return nodeFound;
            }
        }
        return nullptr;
    }

    static fairygui::GObject* findNode(fairygui::GObject* node, const std::string& name) {
        if (!node || name.empty())
            return nullptr;
        if (node->name == name) {
            return node;
        }
        if (auto component = dynamic_cast<fairygui::GComponent*>(node)) {
            fairygui::GObject* nodeFound = component->getChild(name);
            auto foundComponent = dynamic_cast<fairygui::GComponent*>(node);
            if (foundComponent) {
                auto children = foundComponent->getChildren();
                for (auto child : children) {
                    foundComponent = dynamic_cast<fairygui::GComponent*>(findNode(child, name));
                    if (foundComponent)
                        return foundComponent;
                }
            }
        }
        return nullptr;
    }

    class findUtility {
    public:

        template<typename T>
        static T findClosest(int roll, std::vector<std::pair<int, T>> items) {
            if (roll > 100)
                roll = 100;
            if (roll <= 0)
                roll = 1;

            int res = 0, diff = 0, lastIndex = 0;
            for (std::size_t i = 0; i < items.size(); ++i) {
                if (i == 0 || std::abs(roll - items[i].first) < diff) {
                    if (res == items[i].first) {
                        if (randomUtility::generateBetween(1, 2) == 1)
                            lastIndex = i;
                    } else {
                        lastIndex = i;
                    }
                    res = items[i].first;
                    diff = std::abs(roll - items[i].first);
                }
            }

            if (res != 0) {
                return items[lastIndex].second;
            }

            return nullptr;
        }

        template<typename T>
        static int findClosestBetween(T current, T resolution, T step) {
            int cntSteps = 0;
            if (current > resolution) {
                while (resolution + step <= current) {
                    resolution += step;
                    cntSteps++;
                }
            } else {
                while (resolution >= current + step) {
                    current += step;
                    cntSteps++;
                }
            }
            return cntSteps;
        };
    };
}// namespace generic::utilityModule

#endif// GENERIC_FINDUTILITY_H
