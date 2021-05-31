#ifndef COMMON_SIGNALINSTANCE_H
#define COMMON_SIGNALINSTANCE_H

#include <map>
#include <vector>
#include <tuple>
#include <functional>

#define GET_SIGNALS() common::coreModule::signalInstance::getInstance()

namespace common::coreModule {

    template<typename ...Types>
    class signalHolder {
    public:
        void emit(Types... args) {}
        bool operator==(const signalHolder& data) const {
            return data.id == this->id;
        }
        unsigned int id;
    };

    class signalBaseData {
    public:
        signalBaseData() = default;
        virtual ~signalBaseData() = default;
    };

    template<typename ...Types>
    class TypedSignalBaseData : public signalBaseData {
    public:
        explicit TypedSignalBaseData(std::tuple<Types...> data) : signalBaseData(), m_data(data) {}
        std::tuple<Types...> getAllData() {
            return m_data;
        }

    private:
        std::tuple<Types...> m_data;
        signalHolder<Types...>* signal;
        std::vector<std::function<void(Types...)>> listeners;
    };

    class signalInstance {
    public:
        signalInstance();
        ~signalInstance();
        static signalInstance& getInstance();
        void clean();

        template<typename ...Types>
        void addSignal(signalHolder<Types...>* signal) {
            //
        }

    private:
        unsigned int signalId = 0u;
//        std::map<>
    };
}


#endif// COMMON_SIGNALINSTANCE_H
