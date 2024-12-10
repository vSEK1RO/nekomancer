#pragma once

#include <functional>
#include <vector>

namespace nek::core
{
    /**
     * basic reactivity features (watch, unwatch, notify)
     */
    class Reactive
    {
    public:
        /**
         * type of called by notify() function
         */
        using Watcher = std::function<void()>;

        /**
         * copies passed function into object's watchers inner storage
         * @return reference to added watcher, which may be passed into unwatch later
         * @exception Exception::ALREADY_WATCHED if this object already watched by passed function reference
         */
        const Watcher &watch(const Watcher &watcher);
        /**
         * moves passed function into object's watchers inner storage
         * @return reference to added watcher, which may be passed into unwatch later
         */
        const Watcher &watch(Watcher &&watcher);
        /**
         * removes function from object's watchers inner storage
         * @return removed function if watcher existed, else copy passed watcher
         */
        Watcher unwatch(const Watcher &watcher) noexcept;

        virtual ~Reactive() = default;

    protected:
        /**
         * calls every watcher with copy passed params
         */
        void notify() const;

    private:
        std::vector<Watcher> _watchers = {};

        /**
         * compares address of passed reference and addresses in object's watchers inner storage
         * @return iterator of found watcher
         */
        std::vector<Reactive::Watcher>::const_iterator _find(const Watcher &watcher) const noexcept;
    };
}