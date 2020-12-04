#pragma once

#ifdef DANK_PLATFORM_WINDOWS

extern DankEngine::Application* DankEngine::CreateApplication();

int main(int argc, char** argv)
{	
	auto app = DankEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif