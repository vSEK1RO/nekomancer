#pragma once

#include <nek/core/Reactive.hpp>
#include <nek/core/State.hpp>

namespace nek::core
{
    /**
     * reactive object, which computes itself on passed reactive objects notifications
     */
    template <typename T>
    class Computed : public Reactive
    {
    public:
        /**
         * type of computation function. all reactives used in it will be watched by this computed
         */
        using Computer = std::function<T()>;
        /**
         * type of called by notify() function: (old_value, value) => void
         */
        using Watcher = std::function<void(const T &, const T &)>;

        /**
         * Constructs object with passed lambda
         */
        Computed(const Computer &computer)
        {
            _value = (State<T> *)operator new(sizeof(State<T>));
            Reactive::do_track = true;
            try
            {
                new (_value) State<T>(computer());
            }
            catch (...)
            {
                Reactive::do_track = false;
                operator delete(_value);
                throw;
            }
            _value->watch([this]()
                          { Reactive::notify(); });
            _watchers = Reactive::watchTracked([this, computer]()
                                               { _value->set(computer()); });
        }

        /**
         * this operation may be tracked by other reactives
         * @return value of this state
         */
        const T &get() const
        {
            return _value->get();
        }
        /**
         * creates type-independent watcher-wrapper () => void { watcher(old_value, value) } and watches itself by Reactive::watch
         * @return reference to added type-independent watcher-wrapper without args, which may be passed into unwatch later
         * @exception Exception::ALREADY_WATCHED if this object already watched by passed function reference
         */
        const Reactive::Watcher &watch(const Watcher &watcher)
        {
            return _value->watch(watcher);
        }
        /**
         * overload of Reactive::watch, adds passed function into object's watchers inner storage
         * @param watcher reference to type-independent wrapper over type-dependent watcher, which was returned by watch()
         * @return reference to added type-independent watcher-wrapper without args, which may be passed into unwatch later
         * @exception Exception::ALREADY_WATCHED if this object already watched by passed function reference
         */
        const Reactive::Watcher &watch(const Reactive::Watcher &watcher)
        {
            return _value->watch(watcher);
        }
        /**
         * removes function from object's watchers inner storage
         * @param watcher reference to added watcher, which was returned by watch()
         * @return *this
         */
        Computed &unwatch(const Reactive::Watcher &watcher)
        {
            _value->Reactive::unwatch(watcher);
            return *this;
        }
        /**
         * @param watcher reference to added watcher, which was returned by watch()
         * @return copy of function by its reference
         * @throw Exception::NOT_WATCHED if this object isn't watched by passed function reference
         */
        Reactive::Watcher getWatcher(const Reactive::Watcher &watcher) const
        {
            return _value->Reactive::getWatcher(watcher);
        }
        /**
         * @param watcher reference to added watcher, which was returned by watch()
         * @return was object watched by passed function ref
         */
        bool has(const Reactive::Watcher &watcher) const
        {
            return _value->Reactive::has(watcher);
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