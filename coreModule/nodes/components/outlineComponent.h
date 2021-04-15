#ifndef COMMON_OUTLINECOMPONENT_H
#define COMMON_OUTLINECOMPONENT_H

#include "cocos2d.h"

namespace common::coreModule {
	class outlineComponent : public cocos2d::Component {
	public:
		outlineComponent();
		~outlineComponent() override;
		void update(float delta) override;
	};
}


#endif //COMMON_OUTLINECOMPONENT_H
