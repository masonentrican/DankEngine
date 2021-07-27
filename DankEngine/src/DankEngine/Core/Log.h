#pragma once

#include "Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

// Declares the Dank Engine logging capabilities. Defines macros
// to easily use various levels logging through both the engine API
// and client project.

namespace Dank {

	class Log
	{
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};

}

#define DANK_CORE_TRACE(...)   ::Dank::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DANK_CORE_INFO(...)    ::Dank::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DANK_CORE_WARN(...)    ::Dank::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DANK_CORE_ERROR(...)   ::Dank::Log::GetCoreLogger()->error(__VA_ARGS__)
#define DANK_CORE_FATAL(...)   ::Dank::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define DANK_TRACE(...)   ::Dank::Log::GetClientLogger()->trace(__VA_ARGS__)
#define DANK_INFO(...)    ::Dank::Log::GetClientLogger()->info(__VA_ARGS__)
#define DANK_WARN(...)    ::Dank::Log::GetClientLogger()->warn(__VA_ARGS__)
#define DANK_ERROR(...)   ::Dank::Log::GetClientLogger()->error(__VA_ARGS__)
#define DANK_FATAL(...)   ::Dank::Log::GetClientLogger()->critical(__VA_ARGS__)