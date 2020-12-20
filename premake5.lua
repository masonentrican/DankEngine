workspace "DankEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	
	startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "DankEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "DankEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "DankEngine/vendor/ImGui"
IncludeDir["glm"] = "DankEngine/vendor/glm"

include "DankEngine/vendor/GLFW"
include "DankEngine/vendor/Glad"
include "DankEngine/vendor/ImGui"


project "DankEngine"
	location "DankEngine"
	kind "StaticLib"
	staticruntime "on"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "dankpch.h"
	pchsource "DankEngine/src/dankpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"DANK_PLATFORM_WINDOWS",
			"DANK_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "DANK_DEBUG"
		runtime "Debug"
		symbols "on"		

	filter "configurations:Release"
		defines "DANK_RELEASE"
		runtime "Release"
		optimize "on"		

	filter "configurations:Dist"
		defines "DANK_DIST"
		runtime "Release"
		optimize "on"		

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"DankEngine/vendor/spdlog/include",
		"DankEngine/src",
		"DankEngine/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"DankEngine"
	}

	filter "system:windows"

		systemversion "latest"

		defines
		{
			"DANK_PLATFORM_WINDOWS",			
		}

	filter "configurations:Debug"
		defines "DANK_DEBUG"
		runtime "Debug"
		symbols "on"		

	filter "configurations:Release"
		defines "DANK_RELEASE"
		runtime "Release"
		optimize "on"		

	filter "configurations:Dist"
		defines "DANK_DIST"
		runtime "Release"
		optimize "on"
