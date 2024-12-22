#pragma once

#include "Alive.hpp"
#include <nek/core/Json.hpp>
#include <nek/core/Property.hpp>
#include <nek/core/Component.hpp>

namespace nek
{
    struct CAlive : public Alive
    {
        CAlive() = default;

        CAlive(const core::Json::Value &config_);

        CAlive &from(const core::Json::Value &config_) override;
        core::Json::Value toJson() const noexcept override;

        virtual ~CAlive() override = default;
    };
}