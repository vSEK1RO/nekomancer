#pragma once

#include <string>
#include <fstream>
#include <filesystem>
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

            auto config_ = Json::parse(buffer.str());
            Json::validate(config_, Json::parse(R"({
                "name": "nek::Engine",
                "type": "object",
                "properties": {
                    "autofind": {
                        "type": "string"
                    },
                    "components": {
                        "type": "object"
                    },
                    "systems": {
                        "type": "object"
                    }
                },
                "required": ["components", "systems"]
            })"));
            config.emplace(std::move(config_));

            auto slash_pos = path_.rfind('/');
            if (slash_pos != std::string::npos)
            {
                _config_path = path_.substr(0, slash_pos + 1);
            }

            message().set({Observable::Status::INFO, "engine config loaded " + path_});
            return *this;
        }

        Engine &loadComponents()
        {
            auto components_config = _findPlugins("components");

            for (const auto &[name, val] : components_config.items())
            {
                components_config[name] = _config_path + Json::to<std::string>(val);
            }

            component_manager.emplace();
            component_manager().addObservers(_observers);
            component_manager().from(components_config);

            entity_store.emplace();
            entity_store().manager.emplace(&component_manager());
            entity_store().addObservers(_observers);
            return *this;
        }

        Engine &loadSystems()
        {
            auto systems_config = _findPlugins("systems");

            for (const auto &[name, val] : systems_config.items())
            {
                systems_config[name] = _config_path + Json::to<std::string>(val);
            }

            system_manager.emplace();
            system_manager().addObservers(_observers);
            system_manager().from(systems_config);

            for (const auto &[name, val] : systems_config.items())
            {
                systems_config[name] = Json::Value();
            }

            system_store.emplace();
            system_store().manager.emplace(&system_manager());
            system_store().engine.emplace(this);
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
        std::string _config_path;

        Engine() = default;

        Json::Value _findPlugins(const std::string &path_)
        {
            Json::Value ret({});
            if (config().contains("autofind"))
            {
                auto ext = Json::to<std::string>(config().at("autofind"));
                for (const auto &entry : std::filesystem::recursive_directory_iterator(_config_path + path_))
                {
                    if (entry.is_directory())
                    {
                        auto filename = entry.path().filename();
                        auto plugin_path = std::filesystem::path(path_) / filename / (filename.string() + ext);
                        ret[std::move(filename)] = Json::from<std::string>(std::move(plugin_path));
                    }
                }
            }
            else
            {
                ret = config().at("components");
            }
            return ret;
        }
    };
}