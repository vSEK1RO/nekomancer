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
            message().set({Observable::Status::INFO, "engine config loaded " + path_});
            return *this;
        }

        Engine &loadComponents()
        {
            components_manager.emplace();
            components_manager().addObservers(_observers);
            components_manager().from(config().at("components"));
            return *this;
        }

        Engine &loadSystems()
        {
            auto systems_config = config().at("systems");

            systems_manager.emplace();
            systems_manager().addObservers(_observers);
            systems_manager().from(systems_config);

            for (const auto &[name, val] : systems_config.items())
            {
                systems_config[name] = Json::Value();
            }

            systems_store.emplace();
            systems_store().manager.emplace(&systems_manager());
            systems_store().addObservers(_observers);
            systems_store().from(systems_config);
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
    };
}