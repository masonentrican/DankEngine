#pragma once

#ifdef DANK_PLATFORM_WINDOWS
	#ifdef DANK_BUILD_DLL
		#define DANK_API __declspec(dllexport)
	#else
		#define DANK_API __declspec(dllimport)
	#endif
#else
	#error Dank Engine only supports windows.
#endif 
