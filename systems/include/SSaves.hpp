#pragma once

#include <vector>
#include <string>
#include <nek/core/Component/Interface.hpp>

namespace nek
{
    struct SSaves : public IComponent
    {
        virtual void createSave(const std::string &name) const noexcept = 0;
        virtual std::vector<std::string> getSaves() const noexcept = 0;
        virtual void loadSave(const std::string &name) = 0;
    };
}