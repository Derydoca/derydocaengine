#pragma once

#include <spdlog/spdlog.h>
#include <string>
#include <chrono>
#include "Logging/LogLevel.h"
#include "Logging/LogDomain.h"

namespace DerydocaEngine::Logging
{
	struct LogMessage
	{
	public:
		LogLevel Level;
		LogDomain Domain;
		std::chrono::time_point<std::chrono::system_clock> Time;
		std::string Message;
		std::string FileName;
		std::string FunctionName;
		int Line;

		LogMessage();
		LogMessage(const ::spdlog::details::log_msg & msg);
	};
}