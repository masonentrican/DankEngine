#include <Dank.h>

class Sandbox : public Dank::Application
{
public:
	Sandbox() {}
	~Sandbox() {}
};

Dank::Application* Dank::CreateApplication()
{
	return new Sandbox();
}