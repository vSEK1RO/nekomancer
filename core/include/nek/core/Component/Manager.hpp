#pragma once

#include <memory>
#include <unordered_map>
#include <nek/core/Json.hpp>
#include <nek/core/Property.hpp>
#include <nek/core/Component/Base.hpp>

namespace nek::core
{
    class ComponentManager : public IJsonable
    {
    public:
        ComponentManager() = delete;
        ComponentManager(const ComponentManager &rhs) = delete;
        ComponentManager &operator=(const ComponentManager &rhs) = delete;
        ComponentManager(ComponentManager &&rhs) noexcept = default;
        ComponentManager &operator=(ComponentManager &&rhs) noexcept = default;

        ComponentManager(const Json::Value &config_)
        {
            from(config_);
        }

        template <IsComponent T>
        std::shared_ptr<T> create(const std::string &name_, const Json::Value &config_) const
        {
            Component::Info &info = _infos.at(name_);
            Component *ptr = info.construct(&config_);
            ptr->id.emplace(info.id);
            return std::shared_ptr<T>(ptr, info.destruct);
        }

        const Component::Id &id(const std::string &name_) const
        {
            return _infos.at(name_).id;
        }

        bool has(const std::string &name_) const
        {
            return _infos.contains(name_);
        }

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