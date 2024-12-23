#pragma once

#include <nek/core/State.hpp>
#include <nek/core/Observer/IObservable.hpp>

namespace nek::core
{
    struct IObserver
    {
        virtual void log(const Observable::Message &value) const = 0;
        State<Observable::Message>::Watcher getWatcher() const noexcept
        {
            return [this](const Observable::Message &, const Observable::Message &value)
            {
                this->log(value);
            };
        }

        virtual ~IObserver() = default;
    };
}