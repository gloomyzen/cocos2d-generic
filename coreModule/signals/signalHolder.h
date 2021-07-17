#ifndef GENERIC_SIGNALHOLDER_H
#define GENERIC_SIGNALHOLDER_H

#include <functional>
#include <map>
#include <tuple>
#include <vector>

namespace generic::signal {

    class basedSignal {
    public:
        basedSignal() = default;
        virtual ~basedSignal() = default;
    };

    template<typename... Args>
    class signalHolder : public basedSignal {

    public:
        signalHolder() = default;
        ~signalHolder() override = default;

        signalHolder(signalHolder const& /*unused*/) {}

        signalHolder& operator=(signalHolder const& other) {
            if (this != &other) {
                disconnectAll();
            }
            return *this;
        }

        signalHolder(signalHolder&& other) noexcept : _slots(std::move(other._slots)), _current_id(other._current_id) {}

        signalHolder& operator=(signalHolder&& other) noexcept {
            if (this != &other) {
                _slots = std::move(other._slots);
                _current_id = other._current_id;
            }

            return *this;
        }

        int connect(std::function<void(Args...)> const& slot) const {
            _slots.insert(std::make_pair(++_current_id, slot));
            return _current_id;
        }

        template<typename T>
        int connectMember(T* inst, void (T::*func)(Args...)) {
            return connect([=](Args... args) {
                (inst->*func)(args...);
            });
        }

        template<typename T>
        int connectMember(T* inst, void (T::*func)(Args...) const) {
            return connect([=](Args... args) {
                (inst->*func)(args...);
            });
        }

        void disconnect(int id) const {
            _slots.erase(id);
        }

        void disconnectAll() const {
            _slots.clear();
        }

        void emit(Args... p) {
            for (auto const& it : _slots) {
                it.second(p...);
            }
        }

        void emitForAllButOne(int excludedConnectionID, Args... p) {
            for (auto const& it : _slots) {
                if (it.first != excludedConnectionID) {
                    it.second(p...);
                }
            }
        }

        void emitFor(int connectionID, Args... p) {
            auto const& it = _slots.find(connectionID);
            if (it != _slots.end()) {
                it->second(p...);
            }
        }

    private:
        mutable std::map<int, std::function<void(Args...)>> _slots;
        mutable int _current_id{ 0 };
    };

}// namespace generic::signal


#endif// GENERIC_SIGNALHOLDER_H
