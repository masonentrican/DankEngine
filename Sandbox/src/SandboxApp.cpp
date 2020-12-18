#include <Dank.h>
#include "ImGui/ImGui.h"

/*----------------------------------------------------------------------------------
The client side code. This is how you build a client executable using the Dank Engine
-------------------------------------------------------------------------------------*/

// Create a new layer
class TestLayer : public Dank::Layer
{
public:
	TestLayer() : Layer("Test")
	{

	}
	
	void OnUpdate() override
	{
		if (Dank::Input::IsKeyPressed(DANK_KEY_TAB))
			DANK_TRACE("Tab key is pressed");
	}

	void OnEvent(Dank::Event& event) override
	{
		DANK_TRACE("{0}", event);
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Window From Sandbox");
		ImGui::Text("Text from sandbox");
		ImGui::End();

		ImGui::Begin("Window From Sandbox 2");
		ImGui::Text("Text from sandbox 2");
		ImGui::End();
	}
	
};

// Declare the Sandbox Class
class Sandbox : public Dank::Application
{
public:
	Sandbox()
	{
		PushLayer(new TestLayer());
	}

	~Sandbox() {}
};

// Declare and instantiate a Sandbox app from Dank Engine
Dank::Application* Dank::CreateApplication()
{
	return new Sandbox();
}