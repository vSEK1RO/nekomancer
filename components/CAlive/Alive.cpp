#include "Alive.hpp"

namespace nek
{
    Alive::Alive(const Json::Value &config_)
    {
        from(config_);
    }

    Alive &Alive::from(const Json::Value &config_)
    {
        health.emplace(config_.at("health"));
        mana.emplace(config_.at("mana"));
        return *this;
    }

    Json::Value Alive::toJson() const noexcept
    {
        Json::Value json;
        json["health"] = Json::from(health());
        json["mana"] = Json::from(mana());
        return json;
    }
}

extern "C"
{
    IComponent *constructComponent()
    {
        return new nek::Alive();
    }
    void destructComponent(const IComponent *component_ptr_) noexcept
    {
        delete component_ptr_;
    }
}