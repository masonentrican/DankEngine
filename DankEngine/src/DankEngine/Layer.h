#pragma once

#include "DankEngine/Core.h"
#include "DankEngine/Events/Event.h"

// Layers are used to self contain certain functionality. We can have a gui layer,
// a debug layer, a post process layer, etc. Layers are inhertited by new classes
// to be defined by the client in their sandbox app.

namespace Dank {

	class DANK_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetString() const { return _debugName; }
	private:
		std::string _debugName;
	};

}
