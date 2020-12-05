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


// General bitshift definition to give us a bit field.
//
// Example usage: Multiple categories can be applied to an event. By bitshifting multiple
// values we can use the resulting bit field to determine every category.
//
// A result of 0000000000010011 means that events 1,2, and 5 are present. This reduces memory cost
// compared to using int's and allocating a unique value for each category.
#define BIT(x) (1 << x)
