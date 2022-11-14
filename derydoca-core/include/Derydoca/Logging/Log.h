#pragma once

#include <spdlog/spdlog.h>
#include "LogLevel.h"
#include "LogDomain.h"
#include "InternalEngineSink.h"

namespace Derydoca::Logging
{
	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()
		{
			return s_CoreLogger;
		}

		inline static std::shared_ptr<engine_console_sync_mt>& GetConsoleSink()
		{
			return s_EngineConsoleSync;
		}

		inline static std::shared_ptr<spdlog::logger>& GetClientLogger()
		{
			return s_ClientLogger;
		}

		static void PrintIt();

	private:
		static std::shared_ptr<engine_console_sync_mt> s_EngineConsoleSync;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
		static std::shared_ptr<spdlog::logger> s_CoreLogger;

	};
}

#define D_LOG_TRACE(...) ::Derydoca::Logging::Log::GetCoreLogger()->log(spdlog::source_loc(__FILE__, __LINE__, __FUNCTION__), spdlog::level::level_enum::trace, __VA_ARGS__)
#define D_LOG_INFO(...) ::Derydoca::Logging::Log::GetCoreLogger()->log(spdlog::source_loc(__FILE__, __LINE__, __FUNCTION__), spdlog::level::level_enum::info, __VA_ARGS__)
#define D_LOG_WARN(...) ::Derydoca::Logging::Log::GetCoreLogger()->log(spdlog::source_loc(__FILE__, __LINE__, __FUNCTION__), spdlog::level::level_enum::warn, __VA_ARGS__)
#define D_LOG_ERROR(...) ::Derydoca::Logging::Log::GetCoreLogger()->log(spdlog::source_loc(__FILE__, __LINE__, __FUNCTION__), spdlog::level::level_enum::err, __VA_ARGS__)
#define D_LOG_CRITICAL(...) ::Derydoca::Logging::Log::GetCoreLogger()->log(spdlog::source_loc(__FILE__, __LINE__, __FUNCTION__), spdlog::level::level_enum::critical, __VA_ARGS__)

#define LOG_TRACE(...) ::Derydoca::Logging::Log::GetClientLogger()->log(spdlog::source_loc(__FILE__, __LINE__, __FUNCTION__), spdlog::level::level_enum::trace, __VA_ARGS__)
#define LOG_INFO(...) ::Derydoca::Logging::Log::GetClientLogger()->log(spdlog::source_loc(__FILE__, __LINE__, __FUNCTION__), spdlog::level::level_enum::info, __VA_ARGS__)
#define LOG_WARN(...) ::Derydoca::Logging::Log::GetClientLogger()->log(spdlog::source_loc(__FILE__, __LINE__, __FUNCTION__), spdlog::level::level_enum::warn, __VA_ARGS__)
#define LOG_ERROR(...) ::Derydoca::Logging::Log::GetClientLogger()->log(spdlog::source_loc(__FILE__, __LINE__, __FUNCTION__), spdlog::level::level_enum::err, __VA_ARGS__)
#define LOG_CRITICAL(...) ::Derydoca::Logging::Log::GetClientLogger()->log(spdlog::source_loc(__FILE__, __LINE__, __FUNCTION__), spdlog::level::level_enum::critical, __VA_ARGS__)
