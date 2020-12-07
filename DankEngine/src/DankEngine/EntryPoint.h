#pragma once

#ifdef DANK_PLATFORM_WINDOWS

// This is invoked by the sandbox exe to start the engine application. The instance
// of the Dank Engine application obj is created here.

extern Dank::Application* Dank::CreateApplication();

int main(int argc, char** argv)
{
	Dank::Log::Init();
	DANK_CORE_WARN("Initialized Log");
	DANK_INFO("Sandbox has started");

	auto app = Dank::CreateApplication();
	app->Run();
	delete app;
}

#endif