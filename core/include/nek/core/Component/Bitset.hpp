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

        ComponentBitset &fromNames(const std::vector<Component::Name> &names);
        ComponentBitset &fromIds(const std::vector<Component::Id> &ids);
        ComponentBitset &operator|=(const Component::Id &id);
        bool operator==(const ComponentBitset &rhs) const;
        bool has(const Component::Name &name) const;
        bool has(const ComponentBitset &bitset) const;

        ~ComponentBitset() = default;

    private:
        std::vector<std::bitset<8>> _bitset;
    };
}