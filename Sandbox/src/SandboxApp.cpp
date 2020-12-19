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
		ImGui::Begin("Window generated from Sandbox proj");
		ImGui::Text("This window was created from the sandbox project by using the ImGui obj.\n----------------------------------------\n\nImGui::Begin('Window title')\nImGui::Text('Window text')\nImGui::End()");
		ImGui::End();

		ImGui::Begin("Window From Sandbox 2");
		ImGui::Text("Using the ImGui example window, we can turn on docking which allows us more control");
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