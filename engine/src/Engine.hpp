#pragma once

#include <string>
#include <fstream>
#include <stdexcept>
#include <nek/core.hpp>

using namespace nek::core;

namespace nek
{
    class Engine : public IEngine
    {
    public:
        Engine(const Engine &rhs) = delete;
        Engine &operator=(const Engine &rhs) = delete;
        Engine(Engine &&rhs) = default;
        Engine &operator=(Engine &&rhs) = default;

        Engine &loadConfig(const std::string &path_)
        {
            std::ifstream file(path_);
            if (!file.is_open())
            {
                throw std::logic_error("failed to load engine config " + path_);
            }
            std::stringstream buffer;
            buffer << file.rdbuf();
            config.emplace(Json::parse(buffer.str()));
            Json::validate(config(), Json::parse(R"({
                "name": "nek::Engine",
                "type": "object",
                "properties": {
                    "components": {
                        "type": "object"
                    },
                    "systems": {
                        "type": "object"
                    }
                },
                "required": ["components", "systems"]
            })"));
            message().set({Observable::Status::INFO, "engine config loaded" + path_});
            return *this;
        }

        Engine &loadComponents()
        {
            components.emplace();
            std::for_each(_observers.begin(), _observers.end(), [this](const IObserver *observer)
                          { components().addObserver(*observer); });
            components().from(config().at("components"));
            return *this;
        }

        Engine &loadSystems()
        {
            systems.emplace();
            std::for_each(_observers.begin(), _observers.end(), [this](const IObserver *observer)
                          { systems().addObserver(*observer); });
            systems().from(config().at("systems"));
            return *this;
        }

        Engine &addObserver(const IObserver &observer) override
        {
            IObservable::addObserver(observer);
            _observers.push_back(&observer);
            return *this;
        }

        ~Engine() = default;

        inline static Engine &getInstance()
        {
            static Engine engine;
            return engine;
        }

    private:
        Engine() = default;

        std::vector<const IObserver *> _observers;
    };
}