#ifndef COMMON_WINDOWBASE_H
#define COMMON_WINDOWBASE_H

#include "cocos2d.h"
#include "common/coreModule/nodes/nodeProperties.h"
#include <string>
#include <vector>
#include <map>

namespace common::coreModule {
	class windowBase : public nodeProperties<cocos2d::Sprite> {
		class windowBaseData {
		public:
			windowBaseData() = default;
			virtual ~windowBaseData() = default;
		};

		template< typename T >
		class TypedWindowBaseData : public windowBaseData {
		public:
			explicit TypedWindowBaseData (const T& data) : windowBaseData(), m_data(data) {}
			T getData() { return m_data; }
		private:
			T m_data;
		};
	public:
		enum class eWindowState {
			CLOSED = 0,
			OPENING,
			OPENED,
			CLOSING
		};

		windowBase();
		~windowBase();
		CREATE_FUNC(windowBase);

		template <typename T>
		void setData (const std::string& name, T data){
			windowData[name] = std::make_shared<windowBaseData>(new TypedWindowBaseData<T>(data));
		}

		template <typename T>
		T getData(const std::string& name) {
			auto find = windowData.find(name);
			if (find != windowData.end()) {
				auto data = dynamic_cast<TypedWindowBaseData<T>>(find->second.get());
				return data.getData();
			}
			return T();
		}

		eWindowState getCurrentState() { return currentState; }
	private:
		eWindowState currentState = eWindowState::CLOSED;
		std::map<std::string, std::shared_ptr<windowBaseData>> windowData;
	};
}//common::coreModule


#endif //COMMON_WINDOWBASE_H
