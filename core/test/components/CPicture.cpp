#include "CPicture.hpp"

namespace nek
{
    CPicture::CPicture(const Json::Value &config_)
    {
        from(config_);
    }

    void CPicture::mount(const ComponentStore *store_)
    {
        IComponent::mount(store_);
    }

    void CPicture::unmount() const noexcept
    {
        IComponent::unmount();
    }

    CPicture &CPicture::from(const Json::Value &config_)
    {
        return *this;
    }

    Json::Value CPicture::toJson() const noexcept
    {
        return Json::Value();
    }
}