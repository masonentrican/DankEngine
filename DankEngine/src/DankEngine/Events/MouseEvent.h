#pragma once

#include "Event.h"

namespace Dank {

	// MouseMovedEvent class constructs a new instance of Event, assigns the relevant
	// CATEGORY(S), and passes the x and y coordinates into the new instance of MouseMovedEvent.
	//
	// To be used by the EventDispatcher class defined in Event.h
	class DANK_API MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(float x, float y) : _mouseX(x), _mouseY(y) {}

		float GetX() const { return _mouseX; }
		float GetY() const { return _mouseY; }


		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: (" << _mouseX << ", " << _mouseY << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)


	private:
		float _mouseX, _mouseY;
	};

	// MouseScrolledEvent class constructs a new instance of Event, assigns the relevant
	// CATEGORY(S) and TYPE(s), and passes the x and y scroll wheel offsets into the new instance of MouseScrolledEvent.
	//
	// To be used by the EventDispatcher class defined in Event.h
	class DANK_API MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float offsetX, float offsetY) : _offsetX(offsetX), _offsetY(offsetY) {}

		inline float GetXOffset() const { return _offsetX; }
		inline float GetYOffset() const { return _offsetY; }

		std::string  ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: (" << GetXOffset() << ", " << GetYOffset() << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float _offsetX, _offsetY;
	};


	// MouseButtonEvent class constructs a new instance of Event, assigns the relevant
	// CATEGORY(S), and passes the button pressed into the new instance of MouseButtonEvent.
	//
	// To be used by the EventDispatcher class defined in Event.h
	class DANK_API MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return _button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	protected:
		MouseButtonEvent(int button) : _button(button) {}

		int _button;
	};


	// MouseButtonPressedEvent class constructs a new instance of MouseButtonEvent, assigns the relevant
	// TYPE(S), and passes the button pressed into the new instance of MouseButtonPressedEvent.
	//
	// To be used by the EventDispatcher class defined in Event.h
	class DANK_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << _button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};


	// MouseButtonReleasedEvent class constructs a new instance of MouseButtonEvent, assigns the relevant
	// TYPE(S), and passes the button pressed into the new instance of MouseButtonPressedEvent.
	//
	// To be used by the EventDispatcher class defined in Event.h
	class DANK_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << _button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};





}