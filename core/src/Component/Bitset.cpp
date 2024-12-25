#include <nek/core/Component/Bitset.hpp>

#include <algorithm>

namespace nek::core
{
    ComponentBitset &ComponentBitset::fromNames(const std::vector<Component::Name> &names_)
    {
        std::vector<Component::Id> ids(names_.size(), Component::Id());
        std::transform(names_.begin(), names_.end(), ids.begin(), [this](const Component::Name &name)
                       { return manager()->id(name); });
        return fromIds(ids);
    }

    ComponentBitset &ComponentBitset::fromIds(const std::vector<Component::Id> &ids_)
    {
        auto old_bitset = _bitset;
        try
        {
            auto it = ids_.begin();
            auto it_end = ids_.end();
            std::fill(_bitset.begin(), _bitset.end(), std::bitset<8>());
            while (it < it_end)
            {
                (*this) |= (*it++);
            }
        }
        catch (...)
        {
            _bitset = std::move(old_bitset);
            throw;
        }
        return *this;
    }

    ComponentBitset &ComponentBitset::operator|=(const Component::Id &id)
    {
        if (_bitset.size() < id / 8 + 1)
        {
            std::vector<std::bitset<8>> extend(id / 8 + 1 - _bitset.size(), std::bitset<8>());
            _bitset.insert(_bitset.end(), extend.begin(), extend.end());
        }
        _bitset[id / 8][id % 8] = true;
        return *this;
    }

    bool ComponentBitset::operator==(const ComponentBitset &rhs) const
    {
        return _bitset == rhs._bitset;
    }

    bool ComponentBitset::has(const Component::Name &name_) const
    {
        auto &id = manager()->id(name_);
        return _bitset[id / 8][id % 8];
    }
}