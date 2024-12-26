#include "Picture.hpp"
#include <nek/CAlive.hpp>

namespace nek
{
    Picture::Picture(const Json::Value &config_)
    {
        from(config_);
    }

    void Picture::mount()
    {
        IComponent::mount();
        CAlive &alive = store()->get<CAlive>("CAlive");
        path.emplace([&alive]()
                     {
            if (alive.health().get() > 0)
            {
                return "alive.png";
            }
            else
            {
                return "dead.png";
            } });
    }

    void Picture::unmount() noexcept
    {
        IComponent::unmount();
    }

    Picture &Picture::from(const Json::Value &config_)
    {
        return *this;
    }

    Json::Value Picture::toJson() const noexcept
    {
        return Json::Value();
    }
}

extern "C"
{
    IComponent *constructComponent()
    {
        return new nek::Picture();
    }
    void destructComponent(const IComponent *component_ptr_) noexcept
    {
        delete component_ptr_;
    }
}