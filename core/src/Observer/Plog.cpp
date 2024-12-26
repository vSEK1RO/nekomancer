#include <nek/core/Observer/Plog.hpp>

#include <plog/Init.h>
#include <plog/Log.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ColorConsoleAppender.h>

namespace nek::core
{
    void ObserverPlog::log(const Observable::Message &value) const
    {
        switch (value.first)
        {
        case Observable::Status::INFO:
            PLOG_INFO << value.second;
            return;
        case Observable::Status::WARNING:
            PLOG_WARNING << value.second;
            return;
        case Observable::Status::ERROR:
            PLOG_ERROR << value.second;
            return;
        default:
            return;
        }
    }

    ObserverPlog &ObserverPlog::getInstance()
    {
        static ObserverPlog instance;
        return instance;
    }

    ObserverPlog::ObserverPlog()
    {
        static plog::ColorConsoleAppender<plog::TxtFormatter> console_appender;
        plog::init(plog::verbose, &console_appender);
    }
}