#pragma once

#ifdef DANK_PLATFORM_WINDOWS

// This is invoked by the sandbox exe to start the engine application. The instance
// of the Dank Engine application obj is created here.

extern Dank::Application* Dank::CreateApplication();

int main(int argc, char** argv)
{
	Dank::Log::Init();		

	DANK_PROFILE_BEGIN_SESSION("Startup", "profiling/DankProfile-Startup.json");
	auto app = Dank::CreateApplication();
	DANK_PROFILE_END_SESSION();

	DANK_PROFILE_BEGIN_SESSION("Runtime", "profiling/DankProfile-Runtime.json");
	app->Run();
	DANK_PROFILE_END_SESSION();

	DANK_PROFILE_BEGIN_SESSION("Shutdown", "profiling/DankProfile-Shutdown.json");
	delete app;
	DANK_PROFILE_END_SESSION();
}

#endif