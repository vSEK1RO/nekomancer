#include <nek/core/Component/Manager.hpp>

#include <algorithm>
#include <filesystem>
#include <nek/core/Exception.hpp>

namespace nek::core
{
    const Component::Id &ComponentManager::id(const std::string &name_) const
    {
        try
        {
            return _infos.at(name_).id;
        }
        catch (...)
        {
            throw Exception(Exception::COMPONENT_NOT_FOUND, name_);
        }
    }

    const std::string &ComponentManager::name(const Component::Id &id_) const
    {
        auto it = std::find_if(_infos.begin(), _infos.end(), [&](const std::pair<const std::string, Component::Info> &pair)
                               { return pair.second.id == id_; });
        if (it == _infos.end())
        {
            throw Exception(Exception::COMPONENT_NOT_FOUND, std::to_string(id_));
        }
        return it->first;
    }

    bool ComponentManager::has(const std::string &name_) const
    {
        return _infos.contains(name_);
    }

    ComponentManager &ComponentManager::from(const Json::Value &config_)
    {
        _config = config_;
        _from(_config);
        return *this;
    }

    ComponentManager &ComponentManager::_from(const Json::Value &config_)
    {
        Json::validate(config_, Json::parse(R"({
            "name": "nek::core::ComponentManager",
            "type": "object",
            "additionalProperties": {
                "type": "string"
            }
        })"));

        Component::Id id = 0;
        std::unordered_map<std::string, Component::Info> infos;

        for (const auto &[name, path_json] : config_.items())
        {
            auto path = Json::to<std::string>(path_json);
            Component::Construct construct;
            Component::Destruct destruct;
            auto lib = std::make_unique<Poco::SharedLibrary>();
            try
            {
                lib->load(path);
                construct = (Component::Construct)lib->getSymbol("constructComponent");
                destruct = (Component::Destruct)lib->getSymbol("destructComponent");
            }
            catch (const Poco::Exception &e)
            {
                message().set({Observable::Status::WARNING, std::string("failed to load component ") + path + "\n" + e.displayText()});
                continue;
            }

            Component::Info info = {
                .id = id++,
                .construct = construct,
                .destruct = destruct,
            };

            _libs.emplace_back(std::move(lib));
            infos[name] = std::move(info);
            message().set({Observable::Status::INFO, std::string("loaded component ") + path});
        }
        _infos.clear();
        _infos = std::move(infos);
        return *this;
    }

    ComponentManager::~ComponentManager()
    {
        for (auto &lib : _libs)
        {
            if (lib->isLoaded())
            {
                lib->unload();
                message().set({Observable::Status::INFO, std::string("unloaded component ") + lib->getPath()});
            }
            else
            {
                message().set({Observable::Status::WARNING, std::string("failed to unload component ") + lib->getPath()});
            }
        }
    }
}