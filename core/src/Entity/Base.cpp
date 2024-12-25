#include <nek/core/Entity/Base.hpp>

namespace nek::core
{
    Entity::Entity()
    {
        manager.watch([this]()
                      { _store.manager.emplace(manager()); });
    }

    bool Entity::has(const std::vector<Component::Name> &names_) const
    {
        ComponentBitset bitset;
        bitset.manager.emplace(manager());
        bitset.fromNames(names_);
        return bitset == _store.bitset();
    }

    Entity &Entity::addObserver(const IObserver &observer_)
    {
        IObservable::addObserver(observer_);
        _store.addObserver(observer_);
        return *this;
    }

    Entity &Entity::from(const Json::Value &config_)
    {
        _store.from(config_);
        return *this;
    }

    Json::Value Entity::toJson() const noexcept
    {
        return _store.toJson();
    }
}