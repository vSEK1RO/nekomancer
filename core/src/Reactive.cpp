#include <nek/core/Reactive.hpp>

#include <algorithm>

namespace nek::core
{
    Reactive &Reactive::operator=(Reactive &&rhs) noexcept
    {
        if (this != &rhs)
        {
            Reactive temp(std::move(rhs));
            swap(temp);
        }
        return *this;
    }

    void Reactive::swap(Reactive &rhs) noexcept
    {
        std::swap(_watchers, rhs._watchers);
    }

    Reactive &Reactive::unwatch(const Watcher &watcher)
    {
        auto w_it = _find(watcher);
        if (w_it != _watchers.end())
        {
            _watchers.erase(w_it);
        }
        return *this;
    }

    Reactive::Watcher Reactive::getWatcher(const Watcher &watcher) const
    {
        auto w_it = _find(watcher);
        if (w_it == _watchers.end())
        {
            throw Exception(Exception::NOT_WATCHED);
        }
        return *w_it;
    }

    bool Reactive::has(const Watcher &watcher) const
    {
        return _find(watcher) != _watchers.end();
    }

    void Reactive::notify() const
    {
        for (const Watcher &watcher : _watchers)
        {
            watcher();
        }
    }

    void Reactive::trackSelf() const
    {
        if (do_track)
        {
            tracked.insert(const_cast<Reactive *>(this));
        }
    }

    std::vector<std::pair<const Reactive::Watcher *, Reactive *>> Reactive::watchTracked(const Watcher &watcher)
    {
        do_track = false;
        std::vector<std::pair<const Reactive::Watcher *, Reactive *>> watchers;
        watchers.reserve(tracked.size());
        for (Reactive *ptr : tracked)
        {
            watchers.push_back(std::pair(&ptr->watch(watcher), ptr));
        }
        tracked.clear();
        return watchers;
    }

    std::vector<Reactive::Watcher>::const_iterator Reactive::_find(const Watcher &watcher) const
    {
        return std::find_if(_watchers.begin(), _watchers.end(), [&](const Watcher &_watcher)
                            { return &_watcher == &watcher; });
    }
}