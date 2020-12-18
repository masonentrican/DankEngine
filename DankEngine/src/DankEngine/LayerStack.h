#pragma once

#include "DankEngine/Core.h"
#include "DankEngine/Layer.h"

namespace Dank {

	class DANK_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return _layers.begin(); }
		std::vector<Layer*>::iterator end() { return _layers.end(); }
	
	private:
		std::vector<Layer*> _layers;
		int _layerInsertIndex = 0;

	};

}