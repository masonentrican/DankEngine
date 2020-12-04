#pragma once

#ifdef DANK_PLATFORM_WINDOWS

extern Dank::Application* Dank::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Dank::CreateApplication();
	app->Run();
	delete app;
}

#endif