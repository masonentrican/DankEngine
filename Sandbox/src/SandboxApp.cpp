#include <Dank.h>
#include "Platform/OpenGL/OpenGLShader.h"
#include "ImGui/ImGui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ModelTest.h"
#include "DankEngine/Core/EntryPoint.h"



/*----------------------------------------------------------------------------------
The client side code. This is how you build a client executable using the Dank Engine
-------------------------------------------------------------------------------------*/

// Create a new layer

// Declare the Sandbox Class
class Sandbox : public Dank::Application
{
public:
	Sandbox()
	{
		PushLayer(new ModelLayer());
	}

	~Sandbox() {}
};

// Declare and instantiate a Sandbox app from Dank Engine
Dank::Application* Dank::CreateApplication()
{
	return new Sandbox();
}