#pragma once

#include <nek/core/Json.hpp>
#include <nek/core/Property.hpp>
#include <nek/core/Component/Manager.hpp>
#include <nek/core/Component/Store.hpp>
#include <nek/core/Component/Bitset.hpp>
#include <nek/core/Observer/IObservable.hpp>

namespace nek::core
{
    struct Entity : public IJsonable, public IObservable
    {
        using Id = uint32_t;
        Property<const Id> id;
        Property<const ComponentManager *> manager;

        Entity();
        Entity(const Entity &rhs) = default;
        Entity &operator=(const Entity &rhs) = default;
        Entity(Entity &&rhs) noexcept = default;
        Entity &operator=(Entity &&rhs) noexcept = default;

        Entity(const Json::Value &json)
        {
            from(json);
        }

        template <IsIComponent T = IComponent>
        T &get(const Component::Name &name_) const
        {
            return _store.get<T>(name_);
        }
        bool has(const std::vector<Component::Name> &names_) const;

        Entity &addObserver(const IObserver &observer_) override;

        Entity &from(const Json::Value &config_) override;
        Json::Value toJson() const noexcept override;

        ~Entity() = default;

    private:
        ComponentStore _store;
    };
}