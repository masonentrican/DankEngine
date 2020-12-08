#pragma once

#include "DankEngine/Core.h"
#include "DankEngine/Events/Event.h"




namespace Dank {

	class DANK_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetString() const { return _debugName; }
	private:
		std::string _debugName;
	};

}
