#pragma once

#include <nek/core/Property.hpp>
#include <nek/core/State.hpp>
#include <nek/core/Component.hpp>

namespace nek
{
    struct Alive : public core::IComponent
    {
        core::Property<core::State<int>> health;
        core::Property<core::State<int>> mana;

        virtual ~Alive() override = default;
    };
}