#ifndef COMMON_WINDOWBASE_H
#define COMMON_WINDOWBASE_H

#include "cocos2d.h"
#include "common/coreModule/nodes/nodeProperties.h"
#include <string>
#include <vector>
#include <map>

namespace common::coreModule {
	class windowBase : public nodeProperties<cocos2d::Sprite> {
		template <typename T>
		class windowBaseData {
		public:
			std::vector<T> value;
		};
		class windowDataStorage {
		public:
			windowDataStorage() = default;
			~windowDataStorage() = default;
			template <typename T>
			void setValue(const std::string& name, T data) {
				//
			}
		};
	public:
		windowBase();
		~windowBase();
		CREATE_FUNC(windowBase);

		template <typename T>
		void setData (const std::string& name, T data){
			auto storage = new windowDataStorage();
			windowData[name] = *storage;
			windowData[name].setValue<T>(name, data);
		}
	private:
		std::map<std::string, windowDataStorage> windowData;
	};
}//common::coreModule


#endif //COMMON_WINDOWBASE_H
