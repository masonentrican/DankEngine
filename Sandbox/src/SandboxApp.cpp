#include <Dank.h>

/*----------------------------------------------------------------------------------

Base event header

Currently using enums to define event types and event categories to reduce cost
of events as they will be used by virtually everything. This may be replaced with 
a strategy pattern down the line to be more open-closed in principle, allowing for
expansion of event types and functionality without having to modify this header.

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