#pragma once

#include <type_traits>
#include <queue>

namespace nek::core
{
    template <typename T>
    struct IdGenerator
    {
        IdGenerator() = default;
        IdGenerator(const IdGenerator &rhs) = default;
        IdGenerator &operator=(const IdGenerator &rhs) = default;
        IdGenerator(IdGenerator &&rhs) noexcept = default;
        IdGenerator &operator=(IdGenerator &&rhs) noexcept = default;

        T next()
        {
            if (_released.empty())
            {
                return _next++;
            }
            auto id = _released.front();
            try
            {
                _released.pop();
            }
            catch (...)
            {
                _released.push(std::move(id));
                throw;
            }
            return id;
        }

        template <std::common_reference_with<T> U>
        IdGenerator &release(U &&u)
        {
            _released.push(std::forward<U>(u));
            return *this;
        }

        IdGenerator &clear()
        {
            _released = std::queue<T>();
            _next = 0;
            return *this;
        }

    private:
        std::queue<T> _released;
        T _next = 0;
    };
}