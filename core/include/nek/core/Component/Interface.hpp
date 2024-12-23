#pragma once

#include <memory>
#include <cstdint>
#include <type_traits>
#include <nek/core/Json.hpp>
#include <nek/core/State.hpp>
#include <nek/core/Property.hpp>

namespace nek::core
{
    struct IComponent;
    struct ComponentStore;

    namespace Component
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

    struct IComponent : public IJsonable
    {
        Property<const Component::Id> id;
        Property<State<Component::Status>> status{Component::Status::CREATED};

        virtual void mount(const ComponentStore *store_);
        virtual void unmount() const noexcept;
        virtual ~IComponent();

    private:
        const ComponentStore *_store;
    };

    template <typename T>
    concept IsIComponent = std::is_base_of_v<IComponent, T> || std::is_same_v<IComponent, T>;
}