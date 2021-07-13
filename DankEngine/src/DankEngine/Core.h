#pragma once

#include <memory>

#ifdef DANK_PLATFORM_WINDOWS
#if DANK_DYNAMIC_LINK
	#ifdef DANK_BUILD_DLL
		#define DANK_API __declspec(dllexport)
	#else
		#define DANK_API __declspec(dllimport)
	#endif
#else
	#define DANK_API
#endif
#else
	#error Dank Engine only supports windows.
#endif


// Used to bind event callbacks easily. See Application cpp constructor for example
#define DANK_BIND_EVENT(x) std::bind(&x, this, std::placeholders::_1)


// Attempts to assert a given argument. If this fails, it is logged and a WINDOWS SPECIFIC
// debug break is called. Crucial function calls can be wrapped in an assert to add debug
// logs to our custom logger upon failure
//
// Example usage: 
// int x = glfwInit(); 
// DANK_CORE_ASSERT(x, "GLFW failed to init")
//
// Returns an engine log that glfw has failed to initialize
#ifdef DANK_ENABLE_ASSERTS
	#define DANK_ASSERT(x, ...) { if(!(x)) { DANK_ERROR("Assertion Failed: {0}", __VA_ARGS___); __debugbreak(); } }
	#define DANK_CORE_ASSERT(x, ...) { if(!(x)) { DANK_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS___); __debugbreak(); } }
#else
	#define DANK_ASSERT(x, ...)
	#define DANK_CORE_ASSERT(x, ...)
#endif


// General bitshift definition to give us a bit field.
//
// Example usage: Multiple categories can be applied to an event. By bitshifting multiple
// values we can use the resulting bit field to determine every category.
//
// A result of 0000000000010011 means that events 1,2, and 5 are present. This reduces memory cost
// compared to using int's and allocating a unique value for each category.
#define BIT(x) (1 << x)


// Better handle smart pointers so that it's cleaner and can be expanded into an asset manager
// for handling reference counting and creation / deletion easily.
namespace Dank {

	template<typename T>
	using Ref	= std::shared_ptr<T>;

	template<typename T>
	using Scope = std::unique_ptr<T>;

}