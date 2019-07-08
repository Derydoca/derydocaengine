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

	LogMessage::LogMessage(const::spdlog::details::log_msg & msg) :
		level(),
		loggerName(),
		message(),
		fileName(),
		functionName(),
		line(-1)
	{
		if (msg.source.filename)
		{
			line = msg.source.line;
			fileName = std::string(msg.source.filename);
			functionName = std::string(msg.source.funcname);
		}

		loggerName = std::string(msg.logger_name.data());
		message = std::string(msg.payload.data(), msg.payload.size());

		switch (msg.level)
		{
		case spdlog::level::level_enum::critical:
			level = Logging::LogLevel::Critical;
			break;
		case spdlog::level::level_enum::debug:
			level = Logging::LogLevel::Debug;
			break;
		case spdlog::level::level_enum::err:
			level = Logging::LogLevel::Err;
			break;
		case spdlog::level::level_enum::trace:
			level = Logging::LogLevel::Trace;
			break;
		case spdlog::level::level_enum::warn:
			level = Logging::LogLevel::Warn;
			break;
		case spdlog::level::level_enum::info:
		default:
			level = Logging::LogLevel::Info;
			break;
		}
	}

}