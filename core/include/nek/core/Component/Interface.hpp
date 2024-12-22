#pragma once

#include <cstdint>
#include <type_traits>
#include <nek/core/Json.hpp>
// #include <nek/core/State.hpp>
#include <nek/core/Property.hpp>

namespace nek::core
{
    struct IComponent : public IJsonable
    {
        using Id = uint32_t;
        using Construct = IComponent *(*)();
        using Destruct = void (*)(const IComponent *);

        struct Info
        {
            Id id;
            Construct construct;
            Destruct destruct;
        };
        // enum Status : uint8_t
        // {
        //     CREATED,
        //     MOUNTED,
        //     UNMOUNTED,
        //     DELETED,
        // };
        // NLOHMANN_JSON_SERIALIZE_ENUM( Status, {
        //     {Status::CREATED, "CREATED"},
        //     {Status::MOUNTED, "MOUNTED"},
        //     {Status::UNMOUNTED, "UNMOUNTED"},
        //     {Status::DELETED, "DELETED"},
        // })

        Property<const Id> id;
        // Property<State<Status>> status;

        // virtual void mount()
        // {
        //     status().set(Status::MOUNTED);
        // }

        // virtual void unmount()
        // {
        //     status().set(Status::UNMOUNTED);
        // }

        virtual ~IComponent() = default;
        // {
        //     status().set(Status::DELETED);
        // }
    };

    template <typename T>
    concept IsIComponent = std::is_base_of_v<IComponent, T> || std::is_same_v<IComponent, T>;
}