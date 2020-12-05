#pragma once

#include "Event.h"
#include <sstream>

namespace Dank {

	// WindowResizeEvent class constructs a new instance of Event, assigns the relevant
	// CATEGORY(S) and TYPE(s), and passes the width and height into the new instance of WindowResizeEvent.
	//
	// To be used by the EventDispatcher class defined in Event.h
	class DANK_API WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(unsigned int width, unsigned int height) : _width(width), _height(height) {}

		inline unsigned int GetWidth() const { return _width; }
		inline unsigned int GetHeight() const { return _height; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowsResizeEvent: (" << _width << ", " << _height << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)


	private:
		unsigned int _width, _height;
	};

	// Currently has no functional definitions outside of type and category
	class DANK_API WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)	
	};

	// Currently has no functional definitions outside of type and category
	class DANK_API AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	// Currently has no functional definitions outside of type and category
	class DANK_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	// Currently has no functional definitions outside of type and category
	class DANK_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


		
		

}