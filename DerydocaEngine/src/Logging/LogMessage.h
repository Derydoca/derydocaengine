#pragma once

#include <spdlog/spdlog.h>
#include "Logging/LogLevel.h"
#include "Logging/LogDomain.h"

namespace DerydocaEngine::Logging
{

	struct LogMessage
	{
	public:
		LogLevel level;
		LogDomain domain;
		std::chrono::time_point<std::chrono::system_clock> time;
		std::string message;
		std::string fileName;
		std::string functionName;
		int line;

		LogMessage();
		LogMessage(const ::spdlog::details::log_msg & msg);
	};

}