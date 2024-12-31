#pragma once

#include <nek/core.hpp>

namespace nek
{
    struct STick : public core::IComponent
    {
        core::Property<core::Reactive> on_update;
        core::Property<bool> active;

        virtual ~STick() = default;
    };
}