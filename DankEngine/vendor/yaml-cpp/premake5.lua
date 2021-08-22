project "yaml-cpp"
    kind "StaticLib"
    language "C++"    

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

	includedirs
	{
		"include"
	}

    filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "On"

    filter "configurations:Debug"
		runtime "Debug"
		symbols "on"		

	filter "configurations:Release"
		runtime "Release"
		optimize "on"