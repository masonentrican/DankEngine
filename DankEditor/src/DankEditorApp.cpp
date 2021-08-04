#include <Dank.h>
#include "DankEngine/Core/EntryPoint.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "ImGui/ImGui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "DankEditorLayer.h"

namespace Dank
{

	// Declare the Sandbox Class
	class DankEditor : public Dank::Application
	{
	public:
		DankEditor()
			: Application("Dank Editor")
		{
			// PushLayer(new TestLayer());
			PushLayer(new DankEditorLayer());

		}

		~DankEditor() {}
	};

	// Declare and instantiate a Sandbox app from Dank Engine
	Dank::Application* Dank::CreateApplication()
	{
		return new DankEditor();
	}

}