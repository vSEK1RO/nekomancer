#pragma once

#include <functional>
#include <vector>
#include <algorithm>
#include <nek/core/Exception.hpp>

namespace nek::core
{
    /**
     * basic reactivity features (watch, unwatch, notify)
     */
    template <typename T>
    class Reactive
    {
    public:
        /**
         * type of called by notify() function. (old_val, new_val) => void
         */
        using Watcher = std::function<void(const T &, const T &)>;

        /**
         * copies passed function into object's watchers inner storage
         * @return reference to added watcher, which may be passed into unwatch later
         * @exception Exception::ALREADY_WATCHED if this object already watched by passed function reference
         */
        const Watcher &watch(const Watcher &watcher)
        {
            if (_find(watcher) != _watchers.end())
            {
                throw Exception(Exception::ALREADY_WATCHED);
            }
            _watchers.push_back(watcher);
            return *_watchers.rbegin();
        }
        /**
         * moves passed function into object's watchers inner storage
         * @return reference to added watcher, which may be passed into unwatch later
         */
        const Watcher &watch(Watcher &&watcher)
        {
            _watchers.push_back(std::forward<Watcher>(watcher));
            return *_watchers.rbegin();
        }
        /**
         * removes function from object's watchers inner storage
         * @return removed function if watcher existed, else copy passed watcher
         */
        Watcher unwatch(const Watcher &watcher) noexcept
        {
            auto w_it = _find(watcher);
            if (w_it == _watchers.end())
            {
                return watcher;
            }
            Watcher unwatched = *w_it;
            _watchers.erase(w_it);
            return unwatched;
        }

        virtual ~Reactive() = default;

    protected:
        /**
         * calls every watcher with copy passed params
         */
        void notify(T old_val, T new_val) const
        {
            for (const Watcher &watcher : _watchers)
            {
                watcher(old_val, new_val);
            }
        }

    private:
        std::vector<Watcher> _watchers = {};

        /**
         * compares address of passed reference and addresses in object's watchers inner storage
         * @return iterator of found watcher
         */
        auto _find(const Watcher &watcher)
        {
            return std::find_if(_watchers.begin(), _watchers.end(), [&](const Watcher &_watcher)
                                { return &_watcher == &watcher; });
        }
    };
}