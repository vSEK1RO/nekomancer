#pragma once

#include <unordered_set>
#include <functional>
#include <vector>
#include <utility>

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

        Reactive() = default;
        Reactive(const Reactive &) = delete;
        Reactive &operator=(const Reactive &) = delete;
        /**
         * Constructs object from moved
         */
        Reactive(Reactive &&rhs) noexcept;
        /**
         * moves passed object into it
         * @return *this
         */
        Reactive &operator=(Reactive &&rhs) noexcept;
        void swap(Reactive &rhs) noexcept;

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
         * @param watcher reference to added watcher, which was returned by watch()
         * @return removed function if watcher existed, else copy passed watcher
         */
        Watcher unwatch(const Watcher &watcher);
        /**
         * calls every watcher with copy passed params
         */
        void notify() const;

        virtual ~Reactive() = default;

    protected:
        /**
         * flag that tells other reactives that when the state changes they need to be added to tracked
         */
        thread_local inline static bool do_track = false;
        /**
         * set of unique reactives whose state was changed while do_track flag was true
         */
        thread_local inline static std::unordered_set<Reactive *> tracked = {};

        /**
         * adds this reactive to "tracked" set if flag do_track was set true
         */
        void trackSelf() const;
        /**
         * adds passed watcher to every tracked reactive
         * @return vector of references to added watcher for every tracked reactive
         */
        static std::vector<std::pair<const Watcher *, Reactive *>> watchTracked(const Watcher &watcher);

    private:
        std::vector<Watcher> _watchers = {};

        /**
         * compares address of passed reference and addresses in object's watchers inner storage
         * @return iterator of found watcher
         */
        std::vector<Watcher>::iterator _find(const Watcher &watcher) noexcept;
    };
}