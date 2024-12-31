#pragma once

#include <nek/core/Component/Interface.hpp>

extern "C"
{
    nek::core::IComponent *constructComponent();
    void destructComponent(const nek::core::IComponent *component_ptr_) noexcept;
}

#define NEK_COMPONENT_PLUGIN(name)                                        \
    extern "C"                                                            \
    {                                                                     \
        IComponent *constructComponent()                                  \
        {                                                                 \
            return new nek::name();                                       \
        }                                                                 \
        void destructComponent(const IComponent *component_ptr_) noexcept \
        {                                                                 \
            delete component_ptr_;                                        \
        }                                                                 \
    }
