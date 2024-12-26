#include <nek/core/Observer/IObservable.hpp>

#include <algorithm>
#include <nek/core/Observer/Interface.hpp>

namespace nek::core
{
    IObservable &IObservable::addObserver(const IObserver &observer_)
    {
        message().watch(observer_.getWatcher());
        _observers.push_back(&observer_);
        return *this;
    }

    IObservable &IObservable::addObservers(const std::vector<const IObserver *> observers_)
    {
        std::for_each(observers_.begin(), observers_.end(), [this](const IObserver *observer)
                      { this->addObserver(*observer); });
        return *this;
    }
}