#pragma once

#include <memory>
#include <unordered_map>
#include <nek/core/Json.hpp>
#include <nek/core/Exception.hpp>
#include <nek/core/Property.hpp>
#include <nek/core/Component/Interface.hpp>
#include <nek/core/Observer/IObservable.hpp>

namespace nek::core
{
    class ComponentManager : public IJsonable, public IObservable
    {
    public:
        ComponentManager() = default;
        ComponentManager(const ComponentManager &rhs) = delete;
        ComponentManager &operator=(const ComponentManager &rhs) = delete;
        ComponentManager(ComponentManager &&rhs) noexcept = default;
        ComponentManager &operator=(ComponentManager &&rhs) noexcept = default;

        ComponentManager(const Json::Value &config_)
        {
            from(config_);
        }

        template <IsIComponent T = IComponent>
        std::shared_ptr<T> create(const std::string &name_) const
        {
            Component::Info info;
            try
            {
                info = _infos.at(name_);
            }
            catch (...)
            {
                throw Exception(Exception::COMPONENT_NOT_FOUND, name_);
            }
            IComponent *ptr = info.construct();

            ptr->id.emplace(info.id);
            T *casted_ptr = dynamic_cast<T *>(ptr);
            if (casted_ptr == nullptr)
            {
                throw Exception(Exception::COMPONENT_DYNAMIC_CAST, name_);
            }
            return std::shared_ptr<T>(casted_ptr, info.destruct);
        }

        const Component::Id &id(const std::string &name_) const;
        const std::string &name(const Component::Id &id_) const;
        bool has(const std::string &name_) const;

        ComponentManager &from(const Json::Value &config_);
        Json::Value toJson() const noexcept
        {
            return _config;
        }

    private:
        Json::Value _config;
        std::unordered_map<std::string, Component::Info> _infos;

        ComponentManager &_from(const Json::Value &config_);
    };
}