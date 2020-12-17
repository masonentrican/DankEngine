#pragma once

#include "DankEngine/Layer.h"

namespace Dank {

	class DANK_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();
		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);

	private:
		float _time = 0.0f;
	};

	

}