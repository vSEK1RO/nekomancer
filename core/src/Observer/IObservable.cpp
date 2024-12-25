#include <nek/core/Observer/IObservable.hpp>

#include <nek/core/Observer/Interface.hpp>

namespace nek::core
{
    IObservable &IObservable::addObserver(const IObserver &observer)
    {
        message().watch(observer.getWatcher());
        return *this;
    }
}