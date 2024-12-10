#pragma once

#include <functional>
#include <vector>
#include <algorithm>
#include <nek/core/Exception.hpp>

namespace nek::core
{
    template <typename T>
    class Reactive
    {
    public:
        using Watcher = std::function<void(const T &, const T &)>;

        const Watcher &watch(const Watcher &watcher)
        {
            if (_find(watcher) != _watchers.end())
            {
                throw Exception(Exception::ALREADY_WATCHED);
            }
            _watchers.push_back(watcher);
            return *_watchers.rbegin();
        }
        const Watcher &watch(Watcher &&watcher)
        {
            _watchers.push_back(std::forward<Watcher>(watcher));
            return *_watchers.rbegin();
        }
        Watcher unwatch(const Watcher &watcher) noexcept
        {
            auto w_it = _find(watcher);
            Watcher unwatched = *w_it;
            _watchers.erase(w_it);
            return unwatched;
        }

        virtual ~Reactive() = default;

    protected:
        void notify(T old_val, T new_val) const
        {
            for (const Watcher &watcher : _watchers)
            {
                watcher(old_val, new_val);
            }
        }

    private:
        std::vector<Watcher> _watchers = {};

        auto _find(const Watcher &watcher)
        {
            return std::find_if(_watchers.begin(), _watchers.end(), [&](const Watcher &_watcher)
                                { return &_watcher == &watcher; });
        }
    };
}