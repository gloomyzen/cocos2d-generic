#ifndef COMMON_PIVOTCOMPONENT_H
#define COMMON_PIVOTCOMPONENT_H

#include "cocos2d.h"

namespace common::coreModule {
	class pivotComponent : public cocos2d::Component {
	public:
		pivotComponent();
		~pivotComponent() override;
		void update(float delta) override;
		void setPivotPoint(cocos2d::Vec2 point);

	private:
		cocos2d::Vec2 pivotPoint = cocos2d::Vec2::ZERO;
	};
}


#endif //COMMON_PIVOTCOMPONENT_H
