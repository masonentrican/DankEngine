#pragma once

#ifdef DANK_PLATFORM_WINDOWS

extern Dank::Application* Dank::CreateApplication();

int main(int argc, char** argv)
{
	Dank::Log::Init();
	DANK_CORE_WARN("Initialized Log");
	DANK_INFO("App has started");

	auto app = Dank::CreateApplication();
	app->Run();
	delete app;
}

#endif