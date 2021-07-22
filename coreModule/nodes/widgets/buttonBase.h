#ifndef GENERIC_BUTTONBASE_H
#define GENERIC_BUTTONBASE_H

#include "cocos/ui/CocosGUI.h"
#include "cocos2d.h"


namespace generic::coreModule {
    class buttonBase : public cocos2d::ui::Widget {
    public:
        buttonBase();
        ~buttonBase() override;

        void loadTexture(const std::string&);
        void setButtonBgSprite(cocos2d::Sprite*);
        void setChangeColorByClick(bool value) {
            changeColorByClick = value;
        }

    protected:
        bool changeColorByClick = true;
    private:
        cocos2d::Sprite* sprite = nullptr;
    };
}// namespace generic::coreModule


#endif// GENERIC_BUTTONBASE_H
