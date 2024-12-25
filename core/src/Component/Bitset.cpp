#include <nek/core/Component/Bitset.hpp>

#include <algorithm>

namespace nek::core
{
    ComponentBitset &ComponentBitset::from(const std::vector<Component::Name> &names_)
    {
        auto old_bitset = _bitset;
        try
        {
            auto it = names_.begin();
            auto it_end = names_.end();
            while (it < it_end)
            {
                auto &id = manager()->id(*it++);
                if (_bitset.size() < id / 8 + 1)
                {
                    std::vector<std::bitset<8>> extend(id / 8 + 1 - _bitset.size(), std::bitset<8>());
                    _bitset.insert(_bitset.end(), extend.begin(), extend.end());
                }
                _bitset[id / 8][id % 8] = true;
            }
        }
        catch (...)
        {
            _bitset = std::move(old_bitset);
            throw;
        }
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