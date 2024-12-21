#pragma once

#include <memory>
#include <nek/core/Exception.hpp>

namespace nek::core
{
    /**
     * wrapper which provides ability for late initialisation
     */
    template <typename T>
    class Property : public Reactive
    {
    public:
        /**
         * Constructs uninitialized object
         */
        Property() = default;
        Property(const Property &) = delete;
        Property &operator=(const Property &) = delete;
        /**
         * Constructs object from moved
         */
        Property(Property &&rhs) noexcept
            : Reactive(std::move(rhs)), _ptr(std::exchange(rhs._ptr, nullptr)) {};
        /**
         * moves passed object into this
         * @return *this
         */
        Property &operator=(Property &&rhs) noexcept
        {
            if (this != &rhs)
            {
                Property temp(std::move(rhs));
                swap(temp);
            }
            return *this;
        }
        void swap(Property &rhs) noexcept
        {
            std::swap(_ptr, rhs._ptr);
            Reactive::swap(rhs);
        }

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
            Reactive::notify();
            return *this;
        }
        /**
         * wrapped object getter with nullptr safety
         * @return reference to wrapped object
         * @throw Exception::PROPERTY_UNINITIALIZED if not initialized
         */
        T &operator()() const
        {
            Reactive::trackSelf();
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
            Reactive::trackSelf();
            return _ptr.get();
        }

    private:
        std::unique_ptr<T> _ptr;
    };
}