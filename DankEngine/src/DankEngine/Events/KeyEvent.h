#pragma once

#include "Event.h"
#include <sstream>

namespace Dank {


	// KeyEvent class constructs a new instance of Event, assigns the relevant
	// CATEGORY(S), and passes the keycode into the new instance of KeyEvent.
	//
	// To be used by KeyPressedEvents and KeyReleasedEvents
	class DANK_API KeyEvent : public Event {
	public:
		inline int GetKeyCode() const { return _keyCode; }
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		// Construct a new instance of this KeyEvent passing in _keyCode
		KeyEvent(int keycode) : _keyCode(keycode) {}

		int _keyCode;
	};


	// KeyPressedEvent constructs a new instance of KeyEvent, assigns the relevant
	// TYPE(S), and passes the keycode and repeatcount into the new instance of KeyPressedEvent
	//
	// To be used by the EventDispatcher class defined in Event.h
	class DANK_API KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(int keycode, int repeatCount) : KeyEvent(keycode), _repeatCount(repeatCount) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << _keyCode << " (" << _repeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int _repeatCount;
	};


	// KeyReleasedEvent constructs a new instance of KeyEvent, assigns the relevant
	// TYPE(S), and passes the keycode into the new instance of KeyReleasedEvent
	//
	// To be used by the EventDispatcher class defined in Event.h
	class DANK_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << _keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};


}