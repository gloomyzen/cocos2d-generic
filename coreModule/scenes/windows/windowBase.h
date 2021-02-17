#ifndef COMMON_WINDOWBASE_H
#define COMMON_WINDOWBASE_H

#include "cocos2d.h"
#include "common/coreModule/nodes/nodeProperties.h"
#include "common/coreModule/nodes/widgets/soundButton.h"
#include <string>
#include <vector>
#include <map>

namespace common::coreModule {
	class windowBase : public soundButton {
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
		explicit windowBase(std::string );
		~windowBase() override;
		CREATE_FUNC(windowBase);

		template <typename T>
		void setData (const std::string& name, T data){
			windowData[name] = new TypedWindowBaseData<T>(data);
		}

		template <typename T>
		T getData(const std::string& name, const T& defaultData) {
			auto find = windowData.find(name);
			if (find != windowData.end()) {
				auto data = dynamic_cast<TypedWindowBaseData<T>*>(find->second);
				return data->getData();
			}
			return defaultData;
		}

		eWindowState getCurrentState() { return currentState; }

		void setWindowName(const std::string& value) { windowName = value; }
		const std::string& getWindowName() const { return windowName; }
		void closeWindow();
	private:
		void initWindow();

		std::string windowName;
		eWindowState currentState = eWindowState::CLOSED;
		std::map<std::string, windowBaseData*> windowData;
		bool handleMissClick = true;
	};
}//common::coreModule


#endif //COMMON_WINDOWBASE_H
