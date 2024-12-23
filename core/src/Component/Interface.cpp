#include <nek/core/Component/Interface.hpp>

#include <nek/core/Component/Store.hpp>

namespace nek::core
{
    void IComponent::mount(const ComponentStore *store_)
    {
        _store = store_;
        status().set(Component::Status::MOUNTED);
    }

    void IComponent::unmount() const noexcept
    {
        status().set(Component::Status::UNMOUNTED);
    }

    IComponent::~IComponent()
    {
        status().set(Component::Status::DELETED);
    }
}