#pragma once

#include <memory>
#include <nek/core/Exception.hpp>

namespace nek::core
{
    /**
     * wrapper which provides ability for late initialisation
     */
    template <typename T>
    class Property
    {
    public:
        /**
         * Constructs uninitialized object
         */
        Property() = default;
        Property(const Property &) = delete;
        Property &operator=(const Property &) = delete;

        /**
         * Construct initialized object, using emplace constructor arguments
         */
        template <typename... Args>
        Property(Args &&...args) noexcept
        {
            emplace(std::forward<Args>(args)...);
        }

        /**
         * initializes object using constructor arguments
         * @return *this
         * @throw Exception::PROPERTY_INITIALIZED if already initialized
         */
        template <typename... Args>
        Property &emplace(Args &&...args)
        {
            if (_ptr.get() != nullptr)
            {
                throw Exception(Exception::PROPERTY_INITIALIZED);
            }
            _ptr = std::make_unique<T>(std::forward<Args>(args)...);
            return *this;
        }
        /**
         * wrapped object getter with nullptr safety
         * @return reference to wrapped object
         * @throw Exception::PROPERTY_UNINITIALIZED if not initialized
         */
        T &operator()() const
        {
            if (_ptr.get() == nullptr)
            {
                throw Exception(Exception::PROPERTY_UNINITIALIZED);
            }
            return *_ptr;
        }
        /**
         * wrapped object getter without nullptr safety
         * @return ptr to wrapped object
         * @warning you shound check ptr before dereference
         */
        T *get() const noexcept
        {
            return _ptr.get();
        }

    private:
        std::unique_ptr<T> _ptr;
    };
}