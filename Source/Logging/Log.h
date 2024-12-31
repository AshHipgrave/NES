#pragma once

#include "Core/Core.h"

using LogPtr = std::shared_ptr<spdlog::logger>;

class Log
{
public:
    Log(Log const&) = delete;
    void operator=(Log const&) = delete;

    static Log& GetInstance()
    {
        static Log instance;

        return instance;
    }

    LogPtr GetLogger() const;

private:
    Log();

    void Init();

private:
    LogPtr m_Logger = nullptr;
};

#define EMULATOR_LOG_TRACE(...) Log::GetInstance().GetLogger()->trace(__VA_ARGS__);
#define EMULATOR_LOG_DEBUG(...) Log::GetInstance().GetLogger()->debug(__VA_ARGS__);
#define EMULATOR_LOG_INFO(...)  Log::GetInstance().GetLogger()->info(__VA_ARGS__);
#define EMULATOR_LOG_WARN(...)  Log::GetInstance().GetLogger()->warn(__VA_ARGS__);
#define EMULATOR_LOG_ERROR(...) Log::GetInstance().GetLogger()->error(__VA_ARGS__);
#define EMULATOR_LOG_FATAL(...) Log::GetInstance().GetLogger()->critical(__VA_ARGS__); EMULATOR_DEBUG_BREAK(); std::exit(EXIT_FAILURE)