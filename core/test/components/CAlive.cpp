#include "CAlive.hpp"

namespace nek
{
    CAlive::CAlive(const Json::Value &config_)
    {
        from(config_);
    }

    CAlive &CAlive::from(const Json::Value &config_)
    {
        health.emplace(config_.at("health"));
        mana.emplace(config_.at("mana"));
        return *this;
    }

    Json::Value CAlive::toJson() const noexcept
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
        return new nek::CAlive();
    }
    void destructComponent(const IComponent *component_ptr_) noexcept
    {
        delete component_ptr_;
    }
}