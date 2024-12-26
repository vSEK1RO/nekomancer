#include "CPicture.hpp"
#include "Alive.hpp"

namespace nek
{
    CPicture::CPicture(const Json::Value &config_)
    {
        from(config_);
    }

    void CPicture::mount()
    {
        IComponent::mount();
        Alive &alive = store()->get<Alive>("CAlive");
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

    void CPicture::unmount() noexcept
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

extern "C"
{
    IComponent *constructComponent()
    {
        return new nek::CPicture();
    }
    void destructComponent(const IComponent *component_ptr_) noexcept
    {
        delete component_ptr_;
    }
}