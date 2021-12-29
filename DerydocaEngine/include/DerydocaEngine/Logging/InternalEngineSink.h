#pragma once

#include <spdlog/details/null_mutex.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/details/synchronous_factory.h>
#include "DerydocaEngine/Logging/LogMessage.h"

namespace DerydocaEngine::Logging
{
	template<typename Mutex>
	class InternalEngineSink final : public ::spdlog::sinks::base_sink<Mutex>
	{
	public:
		explicit InternalEngineSink() :
			m_Messages()
		{
		}

		std::vector<Logging::LogMessage>& GetMessages()
		{
			return m_Messages;
		}

	protected:
		void sink_it_(const ::spdlog::details::log_msg &msg) override
		{
			m_Messages.push_back(msg);
		}
		void flush_() override
		{
		}

	private:
		std::vector<Logging::LogMessage> m_Messages;

	};

	using engine_console_sync_mt = InternalEngineSink<std::mutex>;
	using engine_console_sync_st = InternalEngineSink<::spdlog::details::null_mutex>;
}
