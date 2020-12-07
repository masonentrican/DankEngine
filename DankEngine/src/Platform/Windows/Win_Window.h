#pragma once

#include "DankEngine/Window.h"
#include "DankEngine/Events/Event.h"

#include "GLFW/glfw3.h"

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