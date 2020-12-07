#pragma once

#include "DankEngine/Window.h"
#include "DankEngine/Events/Event.h"
#include "DankEngine/Events/ApplicationEvent.h"
#include "DankEngine/Events/KeyEvent.h"
#include "DankEngine/Events/MouseEvent.h"

#include "GLFW/glfw3.h"

// Windows specific declarations of the Windows object. Declares
// the various interface functionality of a window on
// windows operating systems. Currently hard coded for GLFW

namespace Dank {

	class Win_Window : public Window
	{
	public:
		Win_Window(const WindowProperties& windowProperties);
		virtual ~Win_Window();

		virtual void OnUpdate() override;

		virtual unsigned int GetWidth() const override { return _windowAttributes.Width; }
		virtual unsigned int GetHeight() const override { return _windowAttributes.Height; }

		virtual void SetEventCallback(const EventCallbackFn& callback) override {
			_windowAttributes.EventCallback = callback;
		};


		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	private:
		struct WindowAttributes
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};


		virtual void Init(const WindowProperties& windowsProperties);
		virtual void Shutdown();

		GLFWwindow* _window;
		WindowAttributes _windowAttributes;

	};

}