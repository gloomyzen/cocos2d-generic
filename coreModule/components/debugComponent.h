#ifndef GENERIC_DEBUGCOMPONENT_H
#define GENERIC_DEBUGCOMPONENT_H

#include "cocos2d.h"

namespace generic::coreModule {

    class debugComponent : public cocos2d::Component {
    public:
        debugComponent();
        ~debugComponent() override;

        void update(float delta) override;

        bool isDebug() const;
        void setDebug(bool);

    protected:
        bool _debugDrawEnabled = false;
        cocos2d::Color4F _debugColorLine = cocos2d::Color4F::WHITE;
        cocos2d::Color4F _debugColorPoint = cocos2d::Color4F::RED;

        cocos2d::DrawNode* _debugDrawNode = nullptr;
    };

}// namespace generic::coreModule

#endif// GENERIC_DEBUGCOMPONENT_H
