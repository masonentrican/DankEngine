#pragma once

#include "dankpch.h"

#include "DankEngine/Core/Core.h"
#include "DankEngine/Events/Event.h"

// Abstract representation of a window for cross platform support

namespace Dank {

	struct WindowProperties
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProperties(const std::string& title = "Dank Engine",
						 uint32_t width = 1280,
						 uint32_t height = 720)
			: Title(title), Width(width), Height(height)
		{
		}

	};

	// Interface representing a desktop system based window
	class DANK_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProperties& properties = WindowProperties());
	};

}