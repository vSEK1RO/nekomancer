#pragma once

#include <nek/core/Json.hpp>
#include <nek/core/Property.hpp>
#include <nek/core/Observer/IObservable.hpp>
#include <nek/core/Component/Manager.hpp>
#include <nek/core/Component/Store.hpp>
#include <nek/core/Entity/Store.hpp>

namespace nek::core
{
    struct IEngine : public IJsonable, public IObservable
    {
        Property<Json::Value> config;
        Property<ComponentManager> component_manager;
        Property<ComponentManager> system_manager;
        Property<ComponentStore> system_store;
        Property<EntityStore> entity_store;

        virtual ~IEngine() = default;
    };
}