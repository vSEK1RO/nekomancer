#pragma once

#include <nek/core/Json.hpp>
#include <nek/core/Property.hpp>
#include <nek/core/Component.hpp>

namespace nek
{
    struct Alive : public core::IComponent
    {
        core::Property<int> health;
        core::Property<int> mana;

        virtual ~Alive() override = default;
    };
}