workspace "DankEngine"
	architecture "x64"
	startproject "DankEditor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


IncludeDir = {}
IncludeDir["GLFW"]       = "DankEngine/vendor/GLFW/include"
IncludeDir["Glad"]       = "DankEngine/vendor/Glad/include"
IncludeDir["ImGui"]      = "DankEngine/vendor/ImGui"
IncludeDir["glm"]        = "DankEngine/vendor/glm"
IncludeDir["stb_image"]  = "DankEngine/vendor/stb_image"
IncludeDir["assimp"]     = "DankEngine/vendor/assimp/include"


group "Dependencies"
	include "DankEngine/vendor/GLFW"
	include "DankEngine/vendor/Glad"
	include "DankEngine/vendor/ImGui"
	

group ""


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
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp"


	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	
	libdirs
	{
		"DankEngine/vendor/assimp/build/lib/Debug"
	}
	
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.assimp}"		
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
		"assimp-vc142-mtd.lib",
		"assimp-vc142-mtd.dll"
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
		"%{IncludeDir.glm}",
		"%{IncludeDir.assimp}"
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


project "DankEditor"
	location "DankEditor"
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
		"%{IncludeDir.glm}",
		"%{IncludeDir.assimp}"
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
