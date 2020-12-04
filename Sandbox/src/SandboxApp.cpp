#include <DankEngine.h>

class Sandbox : public DankEngine::Application
{
public:
	Sandbox()
	{
	}

	~Sandbox()
	{
	}

};

DankEngine::Application* DankEngine::CreateApplication()
{
	return new Sandbox();
}