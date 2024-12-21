#pragma once

#include <cstdint>
#include <type_traits>
#include <nek/core/Json.hpp>
#include <nek/core/Property.hpp>

namespace nek::core
{
    struct Component : public IJsonable
    {
        using Id = uint32_t;
        using Construct = Component *(*)(const Json::Value *);
        using Destruct = int(*)(const Component *);

        struct Info
        {
            Id id;
            Construct construct;
            Destruct destruct;
        };

        Property<const Id> id;

        ~Component() = default;
    };

    template <typename T>
    concept IsComponent = std::is_base_of_v<Component, T>;
}