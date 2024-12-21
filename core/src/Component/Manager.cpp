#include <nek/core/Component/Manager.hpp>

#include <Poco/SharedLibrary.h>
#include <nek/core/Exception.hpp>

namespace nek::core
{
    ComponentManager &ComponentManager::from(const Json::Value &config_)
    {
        _config = config_;
        _from(_config);
        return *this;
    }

    ComponentManager &ComponentManager::_from(const Json::Value &config_)
    {
        if (!config_.is_object())
        {
            throw Exception(Exception::JSON_PROPERTY, "'components' should be object of type\n{\t[name: string]: [path: string]\n}");
        }

        Component::Id id = 0;
        std::unordered_map<std::string, Component::Info> infos;

        for (const auto &[name, path_json] : config_.items())
        {
            auto path = Json::to<std::string>(path_json);
            Component::Construct construct;
            Component::Destruct destruct;
            try
            {
                Poco::SharedLibrary component(path);
                construct = (Component::Construct)component.getSymbol("constructComponent");
                destruct = (Component::Destruct)component.getSymbol("destructComponent");
                if (!(construct && destruct))
                {
                    throw;
                }
            }
            catch (...)
            {
                // FIX log here
                continue;
            }

            Component::Info info = {
                .id = id++,
                .construct = construct,
                .destruct = destruct,
            };

            infos[name] = info;
        }
        _infos.clear();
        _infos = std::move(infos);
        return *this;
    }
}