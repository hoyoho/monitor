#ifndef LOGGER_H
#define LOGGER_H
#include <cstddef>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

class Logger
{
    public:
        Logger(Logger const&)          = delete;
        void operator=(Logger const&)  = delete;
        Logger() = delete;
        static std::shared_ptr<spdlog::logger> getLogger();
};
#endif

