#include <nek/SSaves.hpp>
#include <nek/core.hpp>

using namespace nek::core;

namespace nek
{
    struct Saves : public SSaves
    {
        void createSave(const std::string &name_) const noexcept override
        {
            std::string dir = "saves", warn = "failed to create save\n";
            if (std::filesystem::exists(dir) && !std::filesystem::is_directory(dir))
            {
                message().set({Observable::Status::WARNING, warn + dir + " already exists"});
                return;
            }
            if (!std::filesystem::exists(dir))
            {
                try
                {
                    std::filesystem::create_directory(dir);
                }
                catch (const std::exception &e)
                {
                    message().set({Observable::Status::WARNING, warn + e.what()});
                    return;
                }
            }
            std::ofstream file(dir + "/" + name_ + ".json");
            if (!file)
            {
                message().set({Observable::Status::WARNING, warn + "save file creation"});
                return;
            }
            file << this->stringify();
            file.close();
            message().set({Observable::Status::INFO, "created save " + name_});
        }

        std::vector<std::string> getSaves() const noexcept override
        {
            std::string dir = "saves", warn = "failed to get saves\n";
            std::vector<std::string> saves;
            if (std::filesystem::exists(dir) && std::filesystem::is_directory(dir))
            {
                for (const auto &entry : std::filesystem::directory_iterator(dir))
                {
                    if (entry.is_regular_file() && entry.path().extension() == ".json")
                    {
                        saves.push_back(entry.path().filename());
                    }
                }
            }
            else
            {
                message().set({Observable::Status::WARNING, warn + dir + " directory doesn't exist"});
                return;
            }
            return saves;
        }

        void loadSave(const std::string &) override
        {
            return;
        }
    };
}