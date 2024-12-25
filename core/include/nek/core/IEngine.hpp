#pragma once

#include <nek/core/Json.hpp>
#include <nek/core/Property.hpp>
#include <nek/core/Observer/IObservable.hpp>
#include <nek/core/Component/Manager.hpp>
#include <nek/core/Component/Store.hpp>

namespace nek::core
{
    struct IEngine : public IJsonable, public IObservable
    {
        Property<Json::Value> config;
        Property<ComponentManager> components_manager;
        Property<ComponentManager> systems_manager;
        Property<ComponentStore> systems_store;

        virtual ~IEngine() = default;
    };
}