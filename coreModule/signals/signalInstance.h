#ifndef COMMON_SIGNALINSTANCE_H
#define COMMON_SIGNALINSTANCE_H

#include <map>
#include <vector>
#include <tuple>

#define GET_SIGNALS() common::coreModule::signalInstance::getInstance()

namespace common::coreModule {
    class signalBaseData {
    public:
        signalBaseData() = default;
        virtual ~signalBaseData() = default;
    };

    template<typename ...Types>
    class TypedSignalBaseData : public signalBaseData {
    public:
        explicit TypedSignalBaseData(std::tuple<Types...> data) : signalBaseData(), m_data(data) {}
        std::tuple<Types...> getData() {
            return m_data;
        }

    private:
        std::tuple<Types...> m_data;
    };

    class signalInstance {
    public:
        signalInstance();
        ~signalInstance();
        static signalInstance& getInstance();
        void clean();

    private:
//        std::map<>
    };
}


#endif// COMMON_SIGNALINSTANCE_H
