#include "CAlive.hpp"

using namespace nek::core;

namespace nek
{
    CAlive::CAlive(const Json::Value &config_)
    {
        from(config_);
    }

    CAlive &CAlive::from(const Json::Value &config_)
    {
        health.emplace(Json::to<int>(config_.at("health")));
        mana.emplace(Json::to<int>(config_.at("mana")));
        return *this;
    }

    Json::Value CAlive::toJson() const noexcept
    {
        Json::Value json;
        json["health"] = Json::from<int>(health());
        json["mana"] = Json::from<int>(mana());
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