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

        IComponent::Id id = 0;
        std::unordered_map<std::string, IComponent::Info> infos;

        for (const auto &[name, path_json] : config_.items())
        {
            auto path = Json::to<std::string>(path_json);
            IComponent::Construct construct;
            IComponent::Destruct destruct;
            try
            {
                Poco::SharedLibrary component(path);
                construct = (IComponent::Construct)component.getSymbol("constructComponent");
                destruct = (IComponent::Destruct)component.getSymbol("destructComponent");
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

            IComponent::Info info = {
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