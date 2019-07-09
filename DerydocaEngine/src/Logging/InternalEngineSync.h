#pragma once

#include <spdlog/details/null_mutex.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/details/synchronous_factory.h>
#include "Logging/LogMessage.h"

namespace DerydocaEngine::Logging
{

	template<typename Mutex>
	class InternalEngineSync final : public ::spdlog::sinks::base_sink<Mutex>
	{
	public:
		explicit InternalEngineSync() :
			m_messages()
		{
		}

		std::vector<Logging::LogMessage>& getMessages()
		{
			return m_messages;
		}

	protected:
		void sink_it_(const ::spdlog::details::log_msg &msg) override
		{
			m_messages.push_back(msg);
		}
		void flush_() override
		{
		}

	private:
		std::vector<Logging::LogMessage> m_messages;

	};

	using engine_console_sync_mt = InternalEngineSync<std::mutex>;
	using engine_console_sync_st = InternalEngineSync<::spdlog::details::null_mutex>;

}