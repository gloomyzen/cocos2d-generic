#ifndef COMMON_BUTTONBASE_H
#define COMMON_BUTTONBASE_H

#include "cocos/ui/CocosGUI.h"
#include "cocos2d.h"


namespace common::coreModule {
    class buttonBase : public cocos2d::ui::Widget {
      public:
        buttonBase();
        ~buttonBase() override;

        void loadTexture(const std::string&);

      private:
        cocos2d::Sprite* sprite = nullptr;
    };
}// namespace common::coreModule


#endif// COMMON_BUTTONBASE_H
