#pragma once

#include <nek/CPicture.hpp>
#include <vector>
#include <utility>
#include <nek/core/Reactive.hpp>

using namespace nek::core;

namespace nek
{
    struct Picture : public CPicture
    {
        Picture() = default;

        Picture(const Json::Value &config_);

        void mount() override;
        void unmount() noexcept override;

        Picture &from(const Json::Value &config_) override;
        Json::Value toJson() const noexcept override;

        virtual ~Picture() override = default;
    };
}