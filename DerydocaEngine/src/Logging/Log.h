#pragma once

#include <spdlog/spdlog.h>
#include "Logging/InternalEngineSync.h"
#include "Logging/LogLevel.h"

namespace DerydocaEngine::Logging
{

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()
		{
			return s_coreLogger;
		}

		inline static std::shared_ptr<engine_console_sync_mt>& GetCoreSync()
		{
			return s_coreSync;
		}

		inline static std::shared_ptr<spdlog::logger>& GetClientLogger()
		{
			return s_clientLogger;
		}

	private:
		static std::shared_ptr<engine_console_sync_mt> s_coreSync;
		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;
	};

}

#define D_LOG_TRACE(...) ::DerydocaEngine::Logging::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define D_LOG_INFO(...) ::DerydocaEngine::Logging::Log::GetCoreLogger()->info(__VA_ARGS__)
#define D_LOG_WARN(...) ::DerydocaEngine::Logging::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define D_LOG_ERROR(...) ::DerydocaEngine::Logging::Log::GetCoreLogger()->error(__VA_ARGS__)
#define D_LOG_CRITICAL(...) ::DerydocaEngine::Logging::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define LOG_TRACE(...) ::DerydocaEngine::Logging::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...) ::DerydocaEngine::Logging::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) ::DerydocaEngine::Logging::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::DerydocaEngine::Logging::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::DerydocaEngine::Logging::Log::GetClientLogger()->critical(__VA_ARGS__)