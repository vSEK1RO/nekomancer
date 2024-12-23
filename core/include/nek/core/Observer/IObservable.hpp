#pragma once

#include <cstdint>
#include <utility>
#include <string>
#include <nek/core/State.hpp>
#include <nek/core/Property.hpp>

namespace nek::core
{
    namespace Observable
    {
        enum class Status : uint8_t
        {
            NONE,
            INFO,
            WARNING,
            FATAL,
        };

        using Message = std::pair<Status, std::string>;
    }

    struct IObservable
    {
        Property<State<Observable::Message>> message{std::pair{Observable::Status::NONE, std::string()}};

        virtual ~IObservable() = default;
    };
}