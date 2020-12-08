#pragma once

#include "dankpch.h"
#include "DankEngine/Core.h"


// ------------------------------------------------------------------------------------
//
// Event.h
// 
// Declares the Event base class to be extended by the specific event implementations.
// Declares the Event dispatcher class. Declares and defines various descriptors for
// the event such as type and category. Helper macros provided for easily defining
// the class types and categories in the implementation details.
//
// Notes:
//
// Currently using enums to define event types and event categories to reduce cost
// of events as they will be used by much of the engine. This may be replaced with 
// a strategy pattern down the line to be more open-closed in principle, allowing for
// expansion of event types without having to modify this header.
//
// ------------------------------------------------------------------------------------

namespace Dank {


	// Defines the various event types to describe an Event instance
	// May not need AppTick, AppUpdate, AppRender events - added for future
	// use cases
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
		AppTick, AppUpdate, AppRender
	};


	// Defines the various event categories to describe an Event instance
	// Makes use of bitshifting to allow multiple categories to be defined
	// by a single resulting bitfield.
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication   = BIT(0),
		EventCategoryInput         = BIT(1),
		EventCategoryKeyboard      = BIT(2),
		EventCategoryMouse         = BIT(3),
		EventCategoryMouseButton   = BIT(4)		
	};

	// The base Event class to be extended by specific event type implementations.
	// Class members are virtual so they MUST be defined by the implementation.
	class DANK_API Event
	{
	public:
		virtual EventType GetEventType()    const = 0;
		virtual const char* GetName()       const = 0;
		virtual int GetCategoryFlags()      const = 0;
		virtual std::string ToString()      const { return GetName(); }		// Debug only and should be removed for release/dist to reduce memory allocation

		inline bool IsInCategory(EventCategory category)
		{ 
			return GetCategoryFlags() & category;
		}

		bool Handled = false;
		
	};

	// Dispatches events based on their type by using an Alias Template. Dispatch can be called to check
	// if the given event is of a given type. If so, it executes the function binded.
	//
	// Example Usage in Application Cpp
	// EventDispatcher dispatcher(e);
	// 
	// dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
	class EventDispatcher
	{
		// Template to pass in multiple data types where T is used - see Alias Template
		// EventFn can now be used to return if the reference is of type T.
		template<typename T> using EventFn = std::function<bool(T&)>;    

	public:
		// On construct, create an instance of this class with the reference of the event recieved so that we can
		// call the EventDispatcher with a different EventFn.
		EventDispatcher(Event& event) : _event(event) {}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (_event.GetEventType() == T::GetStaticType())
			{				
				_event.Handled = func(*(T*)&_event);
				return true;
			}
			return false;
		}
	private:
		Event& _event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

// Helper macros to make it easier to define event class type and category in the implementation. Can define multiple categories.
// Without these macros every implementation of Event would need to manually define GetStaticType(), GetEventType(), and GetName()
//
//		Example Usage: EVENT_CLASS_TYPE(KeyPressed)
//					   EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
							   virtual EventType GetEventType() const override { return GetStaticType(); }\
							   virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

}