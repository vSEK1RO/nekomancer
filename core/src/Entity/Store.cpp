#include <nek/core/Entity/Store.hpp>

#include <algorithm>

namespace nek::core
{
    std::shared_ptr<Entity> EntityStore::get(const Entity::Id &id_) const
    {
        if (_entities.size() > id_)
        {
            auto ptr = _entities[id_];
            if (ptr.get())
            {
                return ptr;
            }
        }
        throw Exception(Exception::ENTITY_NOT_EXIST, std::to_string(id_));
    }

    std::vector<std::shared_ptr<Entity>> EntityStore::get(const std::vector<Component::Name> &names_) const
    {
        std::vector<std::shared_ptr<Entity>> res;
        std::for_each(_entities.begin(), _entities.end(), [&](const std::shared_ptr<Entity> &ptr)
                      {
            if (ptr->has(names_))
            {
                res.push_back(ptr);
            } });

        return res;
    }

    std::shared_ptr<Entity> EntityStore::create(const Json::Value &config_)
    {
        auto entity = std::make_shared<Entity>();
        entity->manager.emplace(manager());
        entity->addObservers(_observers);
        entity->from(config_);
        entity->id.emplace(_id_gen.next());
        if (_entities.size() <= entity->id())
        {
            _entities.resize(entity->id() + 1);
        }
        _entities[entity->id()] = entity;
        message().set({Observable::Status::INFO, "created entity with id " + std::to_string(entity->id())});
        return entity;
    }

    EntityStore &EntityStore::remove(const Entity::Id &id_)
    {
        if (_entities.size() <= id_)
        {
            throw Exception(Exception::ENTITY_NOT_EXIST, std::to_string(id_));
        }
        _id_gen.release(id_);
        _entities[id_] = nullptr;
        message().set({Observable::Status::INFO, "removed entity with id " + std::to_string(id_)});
        return *this;
    }

    EntityStore &EntityStore::from(const Json::Value &config_)
    {
        Json::validate(config_, Json::parse(R"({
            "name": "EntityStore",
            "type": "array",
            "items": {
                "type": "object"
            }
        })"));
        auto old_entities = _entities;
        auto old_id_gen = _id_gen;
        try
        {
            _entities.clear();
            _entities.reserve(config_.size());
            _id_gen.clear();
            for (const auto &item : config_)
            {
                create(item);
            }
        }
        catch (...)
        {
            _entities = std::move(old_entities);
            _id_gen = std::move(old_id_gen);
            throw;
        }
        return *this;
    }

    Json::Value EntityStore::toJson() const noexcept
    {
        Json::Value json;
        std::for_each(_entities.begin(), _entities.end(), [&](const std::shared_ptr<Entity> &ptr)
                      {
            if (ptr.get())
            {
                json.push_back(ptr->toJson());
            } });

        return json;
    }
}