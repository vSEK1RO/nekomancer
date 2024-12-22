#pragma once

#include <memory>
#include <unordered_map>
#include <nek/core/Json.hpp>
#include <nek/core/Exception.hpp>
#include <nek/core/Property.hpp>
#include <nek/core/Component/Interface.hpp>

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

        template <IsIComponent T = IComponent>
        std::shared_ptr<T> create(const std::string &name_) const
        {
            try
            {
                const IComponent::Info &info = _infos.at(name_);
                IComponent *ptr = info.construct();

                ptr->id.emplace(info.id);
                T *casted_ptr = dynamic_cast<T *>(ptr);
                if (casted_ptr == nullptr)
                {
                    throw Exception(Exception::COMPONENT_DYNAMIC_CAST, name_);
                }
                return std::shared_ptr<T>(casted_ptr, info.destruct);
            }
            catch(...)
            {
                throw Exception(Exception::COMPONENT_NOT_FOUND, name_);
            }
        }

        const IComponent::Id &id(const std::string &name_) const
        {
            try
            {
                return _infos.at(name_).id;
            }
            catch(...)
            {
                throw Exception(Exception::COMPONENT_NOT_FOUND, name_);
            }
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
        std::unordered_map<std::string, IComponent::Info> _infos;

        ComponentManager &_from(const Json::Value &config_);
    };
}