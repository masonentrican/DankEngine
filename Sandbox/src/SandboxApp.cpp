#include <Dank.h>

/*----------------------------------------------------------------------------------

The client side code. This is all thats requried to create a new game instance
using Dank Engine.

-------------------------------------------------------------------------------------*/

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