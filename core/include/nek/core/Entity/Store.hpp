#pragma once

#include <vector>
#include <memory>
#include <nek/core/Json.hpp>
#include <nek/core/IdGenerator.hpp>
#include <nek/core/Entity/Base.hpp>
#include <nek/core/Observer/IObservable.hpp>
#include <nek/core/Component/Manager.hpp>
#include <nek/core/Component/Bitset.hpp>

namespace nek::core
{
    class EntityStore : public IJsonable, public IObservable
    {
    public:
        Property<const ComponentManager *> manager;

        EntityStore() = default;
        EntityStore(const EntityStore &rhs) = delete;
        EntityStore &operator=(const EntityStore &rhs) = delete;
        EntityStore(EntityStore &&rhs) noexcept = default;
        EntityStore &operator=(EntityStore &&rhs) noexcept = default;

        EntityStore(const Json::Value &config_)
        {
            from(config_);
        }

        std::shared_ptr<Entity> get(const Entity::Id &id_) const;
        std::vector<std::shared_ptr<Entity>> get(const std::vector<Component::Name> &names_) const;
        std::shared_ptr<Entity> create(const Json::Value &config_);
        EntityStore &remove(const Entity::Id &id_);

        EntityStore &from(const Json::Value &config_) override;
        Json::Value toJson() const noexcept override;

        ~EntityStore() = default;

    private:
        std::vector<std::shared_ptr<Entity>> _entities;
        IdGenerator<Entity::Id> _id_gen;
    };
}