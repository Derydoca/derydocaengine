#include "EnginePch.h"
#include "LogMessage.h"

namespace DerydocaEngine::Logging
{

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