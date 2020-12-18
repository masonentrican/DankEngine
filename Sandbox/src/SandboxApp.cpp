#include <Dank.h>

/*----------------------------------------------------------------------------------
The client side code. This is how you build a client executable using the Dank Engine
-------------------------------------------------------------------------------------*/


// Create a new layer
class TestLayer : public Dank::Layer
{
public:
	TestLayer() : Layer("Test") {}
	
	void OnUpdate() override
	{
		if (Dank::Input::IsKeyPressed(DANK_KEY_TAB))
			DANK_TRACE("Tab key is pressed");
	}

	void OnEvent(Dank::Event& event) override
	{
		DANK_TRACE("{0}", event);
	}
};

// Declare the Sandbox Class
class Sandbox : public Dank::Application
{
public:
	Sandbox()
	{
		PushLayer(new TestLayer());
		PushOverlay(new Dank::ImGuiLayer());
	}

	~Sandbox() {}
};

// Declare and instantiate a Sandbox app from Dank Engine
Dank::Application* Dank::CreateApplication()
{
	return new Sandbox();
}