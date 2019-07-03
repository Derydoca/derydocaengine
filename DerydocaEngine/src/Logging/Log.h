#pragma once

#include "spdlog/details/file_helper.h"
#include "spdlog/details/null_mutex.h"
#include "spdlog/sinks/base_sink.h"
#include "spdlog/details/synchronous_factory.h"

#include <mutex>
#include "spdlog/spdlog.h"

namespace DerydocaEngine::Logging
{

	enum LogLevel
	{
		Trace,
		Debug,
		Info,
		Warn,
		Err,
		Critical
	};

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
	};

	//template<typename Mutex>
	//class engine_console_sync final : public base_sink<Mutex>
	//{
	//public:
	//	explicit engine_console_sync();

	//protected:
	//	void sink_it_(const ::spdlog::details::log_msg &msg) override;
	//	void flush_() override;

	//private:
	//	
	//};

	//using engine_console_sync_mt = engine_console_sync<std::mutex>;
	//using engine_console_sync_st = engine_console_sync<::spdlog::details::null_mutex>;

	//template<typename Mutex>
	//inline void engine_console_sync<Mutex>::sink_it_(const ::spdlog::details::log_msg & msg)
	//{
	//}

	//template<typename Mutex>
	//inline void engine_console_sync<Mutex>::flush_()
	//{
	//}
	////
	//// factory functions
	////
	//template<typename Factory = ::spdlog::synchronous_factory>
	//inline std::shared_ptr<::spdlog::logger> basic_logger_mt(const std::string &logger_name, const ::spdlog::filename_t &filename, bool truncate = false)
	//{
	//	return Factory::template create<sinks::engine_console_sync_mt>(logger_name, filename, truncate);
	//}

	//template<typename Factory = ::spdlog::synchronous_factory>
	//inline std::shared_ptr<::spdlog::logger> basic_logger_st(const std::string &logger_name, const ::spdlog::filename_t &filename, bool truncate = false)
	//{
	//	return Factory::template create<sinks::engine_console_sync_st>(logger_name, filename, truncate);
	//}

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()
		{
			return s_coreLogger;
		}

		inline static std::shared_ptr<spdlog::logger>& GetClientLogger()
		{
			return s_clientLogger;
		}
	private:
		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;
	};

}

#define D_LOG_TRACE(...) ::DerydocaEngine::Logging::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define D_LOG_INFO(...) ::DerydocaEngine::Logging::Log::GetCoreLogger()->info(__VA_ARGS__)
#define D_LOG_WARN(...) ::DerydocaEngine::Logging::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define D_LOG_ERROR(...) ::DerydocaEngine::Logging::Log::GetCoreLogger()->error(__VA_ARGS__)
#define D_LOG_CRITICAL(...) ::DerydocaEngine::Logging::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define LOG_TRACE(...) ::DerydocaEngine::Logging::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...) ::DerydocaEngine::Logging::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) ::DerydocaEngine::Logging::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::DerydocaEngine::Logging::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::DerydocaEngine::Logging::Log::GetClientLogger()->critical(__VA_ARGS__)