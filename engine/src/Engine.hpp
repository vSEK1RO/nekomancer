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
            component_manager.emplace();
            component_manager().addObservers(_observers);
            component_manager().from(config().at("components"));

            entity_store.emplace();
            entity_store().manager.emplace(&component_manager());
            entity_store().addObservers(_observers);
            return *this;
        }

        Engine &loadSystems()
        {
            auto systems_config = config().at("systems");

            system_manager.emplace();
            system_manager().addObservers(_observers);
            system_manager().from(systems_config);

            for (const auto &[name, val] : systems_config.items())
            {
                systems_config[name] = Json::Value();
            }

            system_store.emplace();
            system_store().manager.emplace(&system_manager());
            system_store().addObservers(_observers);
            system_store().from(systems_config);
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