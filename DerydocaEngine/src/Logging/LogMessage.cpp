#include "DerydocaEngine/Logging/LogMessage.h"

namespace DerydocaEngine::Logging
{
	LogMessage::LogMessage() :
		Level(),
		Domain(LogDomain::Engine),
		Time(),
		Message(),
		FileName(),
		FunctionName(),
		Line(-1)
	{
	}

	LogMessage::LogMessage(const::spdlog::details::log_msg & msg) :
		Level(),
		Domain(),
		Time(),
		Message(),
		FileName(),
		FunctionName(),
		Line(-1)
	{
		if (msg.source.filename)
		{
			Line = msg.source.line;
			FileName = std::string(msg.source.filename);
			FunctionName = std::string(msg.source.funcname);
		}

		Domain = msg.logger_name.data()[0] == 'E' ? Logging::LogDomain::Engine : Logging::LogDomain::Client;
		Message = std::string(msg.payload.data(), msg.payload.size());
		Time = msg.time;

		switch (msg.level)
		{
		case spdlog::level::level_enum::critical:
			Level = Logging::LogLevel::Critical;
			break;
		case spdlog::level::level_enum::debug:
			Level = Logging::LogLevel::Debug;
			break;
		case spdlog::level::level_enum::err:
			Level = Logging::LogLevel::Err;
			break;
		case spdlog::level::level_enum::trace:
			Level = Logging::LogLevel::Trace;
			break;
		case spdlog::level::level_enum::warn:
			Level = Logging::LogLevel::Warn;
			break;
		case spdlog::level::level_enum::info:
		default:
			Level = Logging::LogLevel::Info;
			break;
		}
	}

}