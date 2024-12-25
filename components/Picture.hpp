#pragma once

#include <string>
#include <nek/core/Property.hpp>
#include <nek/core/Computed.hpp>
#include <nek/core/Component.hpp>

namespace nek
{
    struct Picture : public core::IComponent
    {
        core::Property<core::Computed<std::string>> path;

        virtual ~Picture() override = default;
    };
}