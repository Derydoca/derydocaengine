#include "EnginePch.h"
#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace DerydocaEngine::Logging
{

	std::shared_ptr<engine_console_sync_mt> Log::s_sink;
	std::shared_ptr<spdlog::logger> Log::s_coreLogger;
	std::shared_ptr<spdlog::logger> Log::s_clientLogger;

	void Log::Init()
	{
		s_sink = std::make_shared<engine_console_sync_mt>();
		auto fileSync = std::make_shared<spdlog::sinks::basic_file_sink_mt>("log.txt");

		std::vector<spdlog::sink_ptr> sinks;
		sinks.push_back(s_sink);
		sinks.push_back(fileSync);

		s_coreLogger = std::make_shared<spdlog::logger>("Engine", begin(sinks), end(sinks));
		s_coreLogger->set_level(spdlog::level::trace);

		s_clientLogger = std::make_shared<spdlog::logger>("Client", begin(sinks), end(sinks));
		s_clientLogger->set_level(spdlog::level::trace);
	}

}