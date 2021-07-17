#pragma once

#include "DankEngine/Core/Layer.h"

#include "DankEngine/Events/ApplicationEvent.h"
#include "DankEngine/Events/MouseEvent.h"
#include "DankEngine/Events/KeyEvent.h"

namespace Dank {

	class DANK_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float _time = 0.0f;
	};

	

}