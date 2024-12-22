#pragma once

#include <nek/core/Component/Interface.hpp>

extern "C"
{
    nek::core::IComponent *constructComponent();
    void destructComponent(const nek::core::IComponent *component_ptr_) noexcept;
}