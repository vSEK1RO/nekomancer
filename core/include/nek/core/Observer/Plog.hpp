#pragma once

#include <nek/core/Observer/Interface.hpp>

namespace nek::core
{
    struct ObserverPlog : public IObserver
    {
        ObserverPlog(const ObserverPlog &rhs) = delete;
        ObserverPlog &operator=(const ObserverPlog &rhs) = delete;
        ObserverPlog(ObserverPlog &&rhs) noexcept = default;
        ObserverPlog &operator=(ObserverPlog &&rhs) noexcept = default;

        static ObserverPlog &getInstance();

        void log(const Observable::Message &value) const override;

        ~ObserverPlog() = default;

    private:
        ObserverPlog();
    };
}