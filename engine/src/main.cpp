#include "Engine.hpp"

#include <iostream>
#include <argparse/argparse.hpp>

namespace nek::engine
{
    uint8_t error(const std::string &str, argparse::ArgumentParser &program) noexcept
    {
        std::cerr << "\033[31merror:\t\033[0m"
                  << str << "\n\n"
                  << program << '\n';
        return 1;
    }
}

int main(int argc, char *argv[])
{
    argparse::ArgumentParser program("nek-engine");
    program.add_argument("-p", "--path")
        .help("path to engine config")
        .metavar("STRING");
    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::exception &err)
    {
        return nek::engine::error(err.what(), program);
    }
    std::string path = program.is_used("-p") ? program.get<std::string>("-p") : "nek-config.json";

    auto &plog = nek::core::ObserverPlog::getInstance();

    try
    {
        auto &engine = nek::Engine::getInstance();
        engine.addObserver(plog);
        engine
            .loadConfig(path)
            .loadComponents()
            .loadSystems();
    }
    catch (const std::exception &e)
    {
        plog.log({Observable::Status::ERROR, std::string(e.what())});
    }
}