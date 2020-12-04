workspace "DankEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "DankEngine"
	location "DankEngine"
	kind "SharedLib"
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
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"DANK_PLATFORM_WINDOWS",
			"DANK_BUILD_DLL"
		}

		postbuildcommands 
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox/")
		}

	filter "configurations:Debug"
		defines "DANK_DEBUG"
		symbols "On"		

	filter "configurations:Release"
		defines "DANK_RELEASE"
		symbols "On"		

	filter "configurations:Dist"
		defines "DANK_DIST"
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
		symbols "On"		

	filter "configurations:Release"
		defines "DANK_RELEASE"
		symbols "On"		

	filter "configurations:Dist"
		defines "DANK_DIST"
		symbols "On"