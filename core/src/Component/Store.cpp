#include <nek/core/Component/Store.hpp>

#include <nek/core/Exception.hpp>

namespace nek::core
{
    bool ComponentStore::has(const std::string &name_) const
    {
        return _manager.has(name_) && _components.contains(_manager.id(name_));
    }

    ComponentStore &ComponentStore::from(const Json::Value &config_)
    {
        if (!config_.is_object())
        {
            throw Exception(Exception::JSON_PROPERTY, "'components' should be object of type\n{\t[name: string]: [config: obj]\n}");
        }

        std::map<Component::Id, std::shared_ptr<IComponent>> components;
        Component::Id id;
        std::shared_ptr<IComponent> ptr;

        for (const auto &[name, config_json] : config_.items())
        {
            try
            {
                id = _manager.id(name);
                ptr = _manager.create(name);
                ptr->from(config_json);
                components[id] = ptr;
            }
            catch (...)
            {
                // FIX log here
                continue;
            }
        }
        _components.clear();
        _components = std::move(components);
        return *this;
    }

    Json::Value ComponentStore::toJson() const noexcept
    {
        Json::Value json;
        for (const auto &[id, ptr] : _components)
        {
            json[_manager.name(id)] = Json::from<IComponent>(*ptr);
        }
        return json;
    }
}