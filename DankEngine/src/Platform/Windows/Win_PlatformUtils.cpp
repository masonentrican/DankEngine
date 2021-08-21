#include "dankpch.h"
#include "DankEngine/Utils/PlatformUtils.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "DankEngine/Core/Application.h"

//
namespace Dank
{
	// https://iq.direct/blog/57-displaying-open-file-dialog-using-winapi.html
	// https://docs.microsoft.com/en-us/windows/win32/dlgbox/using-common-dialog-boxes?redirectedfrom=MSDN#open_file
	std::string FileDialogs::OpenFile(const char* filter)
	{
		// Get current working directory
		char tmp[256];
		_getcwd(tmp, 256);

		// Create default path
		char path[] = "\\assets\\scenes";
		char* defaultPath = new char[std::strlen(tmp) + std::strlen(path)];
		std::strcpy(defaultPath, tmp);
		std::strcat(defaultPath, path);

		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrInitialDir = defaultPath;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return std::string();
	}
	// https://iq.direct/blog/57-displaying-open-file-dialog-using-winapi.html
	// https://docs.microsoft.com/en-us/windows/win32/dlgbox/using-common-dialog-boxes?redirectedfrom=MSDN#open_file
	std::string FileDialogs::SaveFile(const char* filter)
	{
		// Get current working directory
		char tmp[256];
		_getcwd(tmp, 256);

		// Create default path
		char path[] = "\\assets\\scenes";
		char* defaultPath = new char[std::strlen(tmp) + std::strlen(path)];
		std::strcpy(defaultPath, tmp);
		std::strcat(defaultPath, path);

		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrInitialDir = defaultPath;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetSaveFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return std::string();
	}

}
