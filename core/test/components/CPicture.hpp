#pragma once

#include "Picture.hpp"
#include <vector>
#include <utility>
#include <nek/core/Reactive.hpp>

using namespace nek::core;

namespace nek
{
    struct CPicture : public Picture
    {
        CPicture() = default;

        CPicture(const Json::Value &config_);

        void mount(const ComponentStore *store_) override;
        void unmount() const noexcept override;

        CPicture &from(const Json::Value &config_) override;
        Json::Value toJson() const noexcept override;

        virtual ~CPicture() override = default;

    private:
        std::vector<std::pair<Reactive *, Reactive::Watcher *>> _watchers;
    };
}