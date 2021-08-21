#pragma once

#include <string>

namespace Dank
{
	class FileDialogs
	{
	public:
		static std::string OpenFile(const char* filter); // Returns empty string if cancelled
		static std::string SaveFile(const char* filter); // Returns empty string if cancelled
	};
}
