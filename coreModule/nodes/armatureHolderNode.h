#ifndef COMMON_ARMATUREHOLDERNODE_H
#define COMMON_ARMATUREHOLDERNODE_H

#include "cocos2d.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"
#include <vector>

namespace common {
	namespace coreModule {
		using namespace dragonBones;

	class armatureHolderNode : public cocos2d::Node {
		public:

			virtual bool init() {
				if(!cocos2d::Node::init())
				{
					return false;
				}

				return true;
			}

			CREATE_FUNC(armatureHolderNode);

			void addArmature(CCArmatureDisplay* armature) {
				_armatureList.emplace_back(armature);
				addChild(armature);
			}
			std::vector<CCArmatureDisplay*> getArmatureList() {
				return _armatureList;
			}
		private:
			std::vector<CCArmatureDisplay*> _armatureList{};
		};
	}
}

#endif //COMMON_ARMATUREHOLDERNODE_H
