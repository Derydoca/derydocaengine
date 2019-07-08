#include "EnginePch.h"
#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace DerydocaEngine::Logging
{

	std::shared_ptr<engine_console_sync_mt> Log::s_coreSync;
	std::shared_ptr<spdlog::logger> Log::s_coreLogger;
	std::shared_ptr<spdlog::logger> Log::s_clientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_coreSync = std::make_shared<engine_console_sync_mt>();
		s_coreLogger = std::make_shared<spdlog::logger>("Engine", s_coreSync);
		s_coreLogger->set_level(spdlog::level::trace);

		s_clientLogger = spdlog::stdout_color_mt("Client");
		s_clientLogger->set_level(spdlog::level::trace);
	}

}