#pragma once

#include <map>
#include <memory>
#include <nek/core/Json.hpp>
#include <nek/core/Component/Manager.hpp>
#include <nek/core/Component/Interface.hpp>

namespace nek::core
{
    class ComponentStore : public IJsonable
    {
    public:
        ComponentStore() = delete;
        ComponentStore(const ComponentStore &rhs) = delete;
        ComponentStore &operator=(const ComponentStore &rhs) = delete;
        ComponentStore(ComponentStore &&rhs) noexcept = default;
        ComponentStore &operator=(ComponentStore &&rhs) noexcept = default;

        ComponentStore(const Json::Value &config_, const ComponentManager *manager_)
            : _manager(manager_)
        {
            from(config_);
        }

        template <IsIComponent T = IComponent>
        T &get(const std::string &name_)
        {
            auto id = _manager->id(name_);
            IComponent *ptr;
            try
            {
                ptr = _components[id].get();
            }
            catch (...)
            {
                throw Exception(Exception::COMPONENT_NOT_FOUND, name_);
            }
            T *casted_ptr = dynamic_cast<T *>(ptr);
            if (casted_ptr == nullptr)
            {
                throw Exception(Exception::COMPONENT_DYNAMIC_CAST, name_);
            }
            return *casted_ptr;
        }
        bool has(const std::string &name_) const;

        ComponentStore &from(const Json::Value &config_) override;
        Json::Value toJson() const noexcept override;

        ~ComponentStore();

    private:
        const ComponentManager *_manager;
        std::map<Component::Id, std::shared_ptr<IComponent>> _components;
    };
}