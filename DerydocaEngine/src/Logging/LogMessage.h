#pragma once

#include <spdlog/spdlog.h>
#include "Logging/LogLevel.h"

namespace DerydocaEngine::Logging
{

	struct LogMessage
	{
	public:
		LogLevel level;
		std::string loggerName;
		//std::string time;
		std::string message;
		std::string fileName;
		std::string functionName;
		int line;

		LogMessage() :
			level(),
			loggerName(),
			message(),
			fileName(),
			functionName(),
			line(-1)
		{
		}

		LogMessage(const ::spdlog::details::log_msg & msg);
	};

}