#pragma once

#include <string>
#include <nek/core/Property.hpp>
#include <nek/core/Computed.hpp>
#include <nek/core/Component.hpp>

namespace nek
{
    struct CPicture : public core::IComponent
    {
        core::Property<core::Computed<std::string>> path;

        virtual ~CPicture() override = default;
    };
}