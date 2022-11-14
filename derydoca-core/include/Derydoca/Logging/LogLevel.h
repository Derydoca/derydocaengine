#pragma once

namespace Derydoca::Logging
{
	enum class LogLevel
	{
		Trace = 1,
		Debug = 2,
		Info = 4,
		Warn = 8,
		Err = 16,
		Critical = 32,
		All = Trace | Debug | Info | Warn | Err | Critical
	};
}
