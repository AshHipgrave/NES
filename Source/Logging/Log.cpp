#include "pch.h"
#include "Logging/Log.h"

Log::Log()
{
    Init();
}

void Log::Init()
{
    auto consoleLogger = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto fileLogger = std::make_shared<spdlog::sinks::basic_file_sink_mt>("Emulator.log", true);

    m_Logger = std::make_shared<spdlog::logger>(spdlog::logger("Emulator", { consoleLogger, fileLogger }));

    m_Logger->set_pattern("%v");
    m_Logger->set_level(spdlog::level::trace);
}

LogPtr Log::GetLogger() const
{
    return m_Logger;
}