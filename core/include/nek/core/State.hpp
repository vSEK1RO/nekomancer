#pragma once

#include <nek/core/Reactive.hpp>
#include <json/json.h>

namespace nek::core
{
    /**
     * reactive object, which stores value: T, default: T
     */
    template <typename T>
    class State : public Reactive
    {
    public:
        /**
         * type of called by notify() function: (old_value, value) => void
         */
        using Watcher = std::function<void(const T &, const T &)>;

        /**
         * set default and value = default
         */
        State(const T &default_) noexcept
            : State(default_, default_) {};
        /**
         * set default and value
         */
        State(const T &default_, const T &value) noexcept
            : _default(default_), _value(value) {};

        /**
         * if Reactive::do_track = true, will be added to Reactive::tracked set after call
         * @return value of this state
         */
        T get() const noexcept
        {
            return _value;
        }
        /**
         * set value of his state
         * @return *this
         */
        State &set(const T &value)
        {
            _value = value;
            notify();
            return *this;
        }
        /**
         * reset value of his state
         * @return *this
         */
        State &reset() noexcept
        {
            _value = _default;
            notify();
            return *this;
        }
        /**
         * creates type-independent watcher-wrapper () => void { watcher(old_value, value) } and watches itself by Reactive::watch
         * @return reference to added type-independent watcher-wrapper without args, which may be passed into unwatch later
         * @exception Exception::ALREADY_WATCHED if this object already watched by passed function reference
         */
        const Reactive::Watcher &watch(const Watcher &watcher)
        {
            return Reactive::watch([this, watcher]()
                                   {
                static T old_value = _default;
                watcher(old_value, _value);
                old_value = _value; });
        }
        /**
         * overload of Reactive::watch, adds passed function into object's watchers inner storage
         * @param watcher reference to type-independent wrapper over type-dependent watcher, which was returned by watch()
         * @return reference to added type-independent watcher-wrapper without args, which may be passed into unwatch later
         * @exception Exception::ALREADY_WATCHED if this object already watched by passed function reference
         */
        const Reactive::Watcher &watch(const Reactive::Watcher &watcher)
        {
            return Reactive::watch(watcher);
        }

    private:
        T _default;
        T _value;
    };
}