#ifndef COMMON_GRIDNODE_H
#define COMMON_GRIDNODE_H

#include "cocos2d.h"
#include "common/coreModule/nodes/nodeProperties.h"
#include <map>
#include <string>
#include <vector>

namespace common {
    namespace coreModule {

        class gridNode
            : public nodeProperties
            , public cocos2d::Node {
          public:
            struct sGridCell {
                cocos2d::Node* node = nullptr;
                cocos2d::Size size;
                sGridCell(cocos2d::Node* _node, cocos2d::Size _size) {
                    node = _node;
                    size = _size;
                }
            };
            enum class eGridDirection {
                HORIZONTAL = 0,
                VERTICAL,
            };
            enum class eGridAlignX {
                TOP = 0,
                BOTTOM,
            };
            enum class eGridAlignY {
                LEFT = 0,
                RIGHT,
            };
            typedef std::map<int, sGridCell*> gridCells;
            typedef std::map<int, gridCells> gridTable;
            typedef std::vector<sGridCell*> gridList;

            gridNode();
            ~gridNode() override;
            CREATE_FUNC(gridNode);

            void updateGridTransform();

            void setMarginX(int X, int Y) {
                marginX.first = X;
                marginX.second = Y;
            }
            void setMarginX(std::pair<int, int> value) { marginX = value; }
            void setMarginY(int X, int Y) {
                marginY.first = X;
                marginY.second = Y;
            }
            void setMarginY(std::pair<int, int> value) { marginY = value; }
            void setPaddingX(int X, int Y) {
                paddingX.first = X;
                paddingX.second = Y;
            }
            void setPaddingX(std::pair<int, int> value) { paddingX = value; }
            void setPaddingY(int X, int Y) {
                paddingY.first = X;
                paddingY.second = Y;
            }
            void setPaddingY(std::pair<int, int> value) { paddingY = value; }
            void setDirection(eGridDirection value) { direction = value; }
            void setAlignX(eGridAlignX value) { alignX = value; }
            void setAlignY(eGridAlignY value) { alignY = value; }
            void setColumns(int value) { columns = value; }
            void setRows(int value) { rows = value; }

            static eGridDirection getGridDirectionByString(const std::string&);
            static eGridAlignX getGridAlignXByString(const std::string&);
            static eGridAlignY getGridAlignYByString(const std::string&);


          private:
            std::pair<int, int> marginX;
            std::pair<int, int> marginY;
            std::pair<int, int> paddingX;
            std::pair<int, int> paddingY;
            eGridDirection direction = eGridDirection::VERTICAL;
            eGridAlignX alignX = eGridAlignX::TOP;
            eGridAlignY alignY = eGridAlignY::LEFT;
            int columns = 1;
            int rows = 1;
        };
    }// namespace coreModule
}// namespace common


#endif// COMMON_GRIDNODE_H
