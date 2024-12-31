#include <nek/core/Component/Interface.hpp>

#include <nek/core/Component/Store.hpp>

namespace nek::core
{
    void IComponent::mount()
    {
        status().set(Component::Status::MOUNTED);
    }

    void IComponent::unmount() noexcept
    {
        status().set(Component::Status::UNMOUNTED);
    }

    IComponent::~IComponent()
    {
        status().set(Component::Status::DELETED);
    }
}