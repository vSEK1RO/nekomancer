#include <nek/STick.hpp>
#include <nek/core/Component/Plugin.hpp>

#include <chrono>
#include <thread>

using namespace nek::core;
using namespace std::chrono;

namespace nek
{
    struct Tick : public STick
    {
        Tick()
        {
            on_update.emplace();
            active.emplace(true);
        }

        void mount() override
        {
            IComponent::mount();
            while (active())
            {
                auto start_time = steady_clock::now();
                on_update.lock();
                on_update().notify();
                on_update.unlock();
                auto end_time = steady_clock::now();
                auto elapsed = duration_cast<microseconds>(end_time - start_time).count();
                auto sleep_time = microseconds(1000) - microseconds(elapsed);
                if (sleep_time > microseconds(0))
                {
                    std::this_thread::sleep_for(sleep_time);
                }
            }
        }

        void unmount() noexcept override
        {
            IComponent::unmount();
        }

        virtual ~Tick() = default;
    };
}

NEK_COMPONENT_PLUGIN(Tick)