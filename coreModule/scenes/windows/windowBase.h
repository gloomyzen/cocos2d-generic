#ifndef COMMON_WINDOWBASE_H
#define COMMON_WINDOWBASE_H

#include "cocos2d.h"
#include "common/coreModule/nodes/nodeProperties.h"

namespace common::coreModule {
	class windowBase : public nodeProperties<cocos2d::Sprite> {
//		template <typename T>
//		class window
	public:
		windowBase();
		~windowBase();
		CREATE_FUNC(windowBase);
	};
}//common::coreModule


#endif //COMMON_WINDOWBASE_H
