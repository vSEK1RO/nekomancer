#pragma once

#include <bitset>
#include <vector>
#include <nek/core/Property.hpp>
#include <nek/core/Component/Interface.hpp>
#include <nek/core/Component/Manager.hpp>

namespace nek::core
{
    class ComponentBitset
    {
    public:
        Property<const ComponentManager *> manager;

        ComponentBitset() = default;
        ComponentBitset(const ComponentBitset &rhs) = default;
        ComponentBitset &operator=(const ComponentBitset &rhs) = default;
        ComponentBitset(ComponentBitset &&rhs) noexcept = default;
        ComponentBitset &operator=(ComponentBitset &&rhs) noexcept = default;

        ComponentBitset &from(const std::vector<Component::Name> &names);
        bool operator==(const ComponentBitset &rhs) const;
        bool has(const Component::Name &name) const;

        ~ComponentBitset() = default;

    private:
        std::vector<std::bitset<8>> _bitset;
    };
}