#pragma once

#include "Alive.hpp"

using namespace nek::core;

namespace nek
{
    struct CAlive : public Alive
    {
        CAlive() = default;

        CAlive(const Json::Value &config_);

        CAlive &from(const Json::Value &config_) override;
        Json::Value toJson() const noexcept override;

        virtual ~CAlive() override = default;
    };
}