#pragma once

#include <nek/core/Json.hpp>
#include <nek/core/Component/Base.hpp>

extern "C"
{
    nek::core::Component *constructComponent(const nek::core::Json::Value *config_);
    int destructComponent(const nek::core::Component *component_ptr_) noexcept;
}