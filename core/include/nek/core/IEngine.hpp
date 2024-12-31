#pragma once

#include <nek/core/Json.hpp>
#include <nek/core/Property.hpp>
#include <nek/core/Observer/IObservable.hpp>

namespace nek::core
{
    struct ComponentManager;
    struct ComponentStore;
    struct EntityStore;

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