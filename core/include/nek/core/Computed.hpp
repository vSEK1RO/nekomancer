#pragma once

#include <nek/core/Reactive.hpp>
#include <nek/core/State.hpp>

namespace nek::core
{
    template <typename T>
    class Computed : public Reactive
    {
    public:
        using Computer = std::function<T()>;
        using Watcher = std::function<void(const T &, const T &)>;

        Computed(const Computer &computer)
        {
            _value = (State<T> *)operator new(sizeof(State<T>));
            Reactive::do_track = true;
            new (_value) State<T>(computer());
            _watchers = Reactive::watchTracked([this, computer]()
                                               { _value->set(computer()); });
        }

        const T &get() const
        {
            return _value->get();
        }
        const Reactive::Watcher &watch(const Watcher &watcher)
        {
            return _value->watch(watcher);
        }
        const Reactive::Watcher &watch(const Reactive::Watcher &watcher)
        {
            return _value->watch(watcher);
        }
        Reactive::Watcher unwatch(const Reactive::Watcher &watcher)
        {
            return _value->Reactive::unwatch(watcher);
        }

        ~Computed()
        {
            for (auto pair : _watchers)
            {
                pair.second->unwatch(*pair.first);
            }
            operator delete(_value);
        }

    private:
        State<T> *_value = nullptr;
        std::vector<std::pair<const Reactive::Watcher *, Reactive *>> _watchers;
    };
}