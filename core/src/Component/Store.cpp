#include <nek/core/Component/Store.hpp>

#include <nek/core/Exception.hpp>

namespace nek::core
{
    bool ComponentStore::has(const std::string &name_) const
    {
        return manager()->has(name_) && _components.contains(manager()->id(name_));
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
                id = manager()->id(name);
                ptr = manager()->create(name);
                ptr->addObservers(_observers);
                ptr->from(config_json);
                components[id] = ptr;
            }
            catch (const std::exception &e)
            {
                message().set({Observable::Status::WARNING, std::string("failed to create component ") + name + "\n" + e.what()});
                continue;
            }
            message().set({Observable::Status::INFO, std::string("created component ") + name});
        }

        _components.clear();

        for (const auto &[id, ptr] : components)
        {
            try
            {
                ptr->store.emplace(this);
                ptr->mount();
                _components[id] = ptr;
                _bitset |= id;
            }
            catch (const std::exception &e)
            {
                message().set({Observable::Status::WARNING, std::string("failed to mount component ") + manager()->name(id) + "\n" + e.what()});
                continue;
            }
            message().set({Observable::Status::INFO, std::string("mounted component ") + manager()->name(id)});
        }
        return *this;
    }

    Json::Value ComponentStore::toJson() const noexcept
    {
        Json::Value json;
        for (const auto &[id, ptr] : _components)
        {
            json[manager()->name(id)] = Json::from<IComponent>(*ptr);
        }
        return json;
    }

    ComponentStore::~ComponentStore()
    {
        for (const auto &[id, ptr] : _components)
        {
            if (ptr.get())
            {
                ptr->unmount();
                message().set({Observable::Status::INFO, std::string("unmounted component ") + manager()->name(id)});
            }
        }
    }
}