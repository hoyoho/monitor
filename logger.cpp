#include "logger.h"
#include <cstddef>
#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

/*--------------------------------------
                        Singletone
---------------------------------------*/
using namespace std;
std::shared_ptr<spdlog::logger> Logger::getLogger()
{
    static auto  aLogger = spdlog::stdout_color_mt("console");
    spdlog::set_pattern("[%H:%M:%S %z] [%^---%L---%$] [thread %t] %v");
    aLogger->set_level(spdlog::level::trace); // Set specific logger's log level
    return aLogger;
}


