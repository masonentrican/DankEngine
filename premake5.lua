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

include "DankEngine/vendor/GLFW"
include "DankEngine/vendor/Glad"
include "DankEngine/vendor/ImGui"


project "DankEngine"
	location "DankEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "dankpch.h"
	pchsource "DankEngine/src/dankpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"DANK_PLATFORM_WINDOWS",
			"DANK_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands 
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox/")
		}

	filter "configurations:Debug"
		defines "DANK_DEBUG"
		buildoptions "/MDd"
		symbols "On"		

	filter "configurations:Release"
		defines "DANK_RELEASE"
		buildoptions "/MD"
		symbols "On"		

	filter "configurations:Dist"
		defines "DANK_DIST"
		buildoptions "/MD"
		symbols "On"		

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

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
		"DankEngine/src"
	}

	links
	{
		"DankEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"DANK_PLATFORM_WINDOWS",			
		}

	filter "configurations:Debug"
		defines "DANK_DEBUG"
		buildoptions "/MDd"
		symbols "On"		

	filter "configurations:Release"
		defines "DANK_RELEASE"
		buildoptions "/MD"
		symbols "On"		

	filter "configurations:Dist"
		defines "DANK_DIST"
		buildoptions "/MD"
		symbols "On"