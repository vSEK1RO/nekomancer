#pragma once

#include <nek/CAlive.hpp>

using namespace nek::core;

namespace nek
{
    struct Alive : public CAlive
    {
        Alive() = default;

        Alive(const Json::Value &config_);

        Alive &from(const Json::Value &config_) override;
        Json::Value toJson() const noexcept override;

        virtual ~Alive() override = default;
    };
}