#pragma once

#include <nek/core/Json.hpp>
#include <nek/core/Property.hpp>
#include <nek/core/Component/Plugin.hpp>

namespace nek
{
    struct CAlive : public core::IComponent
    {
        core::Property<int> health;
        core::Property<int> mana;

        CAlive() = default;

        CAlive(const core::Json::Value &config_);

        CAlive &from(const core::Json::Value &config_) override;
        core::Json::Value toJson() const noexcept override;

        ~CAlive() override = default;
    };
}