#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace DerydocaEngine::Logging
{
	std::shared_ptr<engine_console_sync_mt> Log::s_EngineConsoleSync;
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		s_EngineConsoleSync = std::make_shared<engine_console_sync_mt>();
		auto fileSync = std::make_shared<spdlog::sinks::basic_file_sink_mt>("log.txt");

		std::vector<spdlog::sink_ptr> sinks;
		sinks.push_back(s_EngineConsoleSync);
		sinks.push_back(fileSync);

		s_CoreLogger = std::make_shared<spdlog::logger>("Engine", begin(sinks), end(sinks));
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>("Client", begin(sinks), end(sinks));
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}
