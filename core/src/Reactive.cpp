#include <nek/core/Reactive.hpp>

#include <algorithm>

namespace nek::core
{
    Reactive::Reactive(Reactive &&rhs) noexcept
    {
        swap(rhs);
    }

    Reactive &Reactive::operator=(Reactive &&rhs) noexcept
    {
        swap(rhs);
        return *this;
    }

    void Reactive::swap(Reactive &rhs) noexcept
    {
        std::swap(_watchers, rhs._watchers);
    }

    Reactive::Watcher Reactive::unwatch(const Watcher &watcher)
    {
        auto w_it = _find(watcher);
        if (w_it == _watchers.end())
        {
            return watcher;
        }
        Watcher unwatched = *w_it;
        try
        {
            _watchers.erase(w_it);
        }
        catch (...)
        {
            *w_it = std::move(unwatched);
            throw;
        }
        return unwatched;
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

    std::vector<Reactive::Watcher>::iterator Reactive::_find(const Watcher &watcher) noexcept
    {
        return std::find_if(_watchers.begin(), _watchers.end(), [&](const Watcher &_watcher)
                            { return &_watcher == &watcher; });
    }
}