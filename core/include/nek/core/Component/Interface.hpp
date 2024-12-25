#pragma once

#include <memory>
#include <cstdint>
#include <type_traits>
#include <nek/core/Json.hpp>
#include <nek/core/State.hpp>
#include <nek/core/Property.hpp>
#include <nek/core/Observer/IObservable.hpp>

namespace nek::core
{
    struct IEngine;
    struct IComponent;
    struct ComponentStore;

    namespace Component
    {
        using Id = uint32_t;
        using Name = std::string;
        using Construct = IComponent *(*)();
        using Destruct = void (*)(const IComponent *);

        struct Info
        {
            Id id;
            Construct construct;
            Destruct destruct;
        };

        enum class Status : uint8_t
        {
            CREATED,
            MOUNTED,
            UNMOUNTED,
            DELETED,
        };
        NLOHMANN_JSON_SERIALIZE_ENUM(Status, {
                                                 {Status::CREATED, "CREATED"},
                                                 {Status::MOUNTED, "MOUNTED"},
                                                 {Status::UNMOUNTED, "UNMOUNTED"},
                                                 {Status::DELETED, "DELETED"},
                                             })
    }

    struct IComponent : public IJsonable, public IObservable
    {
        Property<const Component::Id> id;
        Property<const IEngine *> engine;
        Property<const ComponentStore *> store;
        Property<State<Component::Status>> status{Component::Status::CREATED};

        virtual void mount();
        virtual void unmount() const noexcept;
        virtual ~IComponent();
    };

    template <typename T>
    concept IsIComponent = std::is_base_of_v<IComponent, T> || std::is_same_v<IComponent, T>;
}