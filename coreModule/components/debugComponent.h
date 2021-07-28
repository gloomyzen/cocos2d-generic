#ifndef GENERIC_DEBUGCOMPONENT_H
#define GENERIC_DEBUGCOMPONENT_H

#include "cocos2d.h"

namespace generic::coreModule {

    class debugComponent : public cocos2d::Component {
    public:
        debugComponent();
        ~debugComponent() override;

        void update(float delta) override;
    };

}// namespace generic::coreModule

#endif// GENERIC_DEBUGCOMPONENT_H
