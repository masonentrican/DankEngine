#pragma once

#include "Scene.h"

namespace Dank
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void SerializeToYAML(const std::string& filepath);
		void SerializeToBinary(const std::string& filepath);

		bool DeserializeYAML(const std::string& filepath);
		bool DeserializeBinary(const std::string& filepath);
	private:
		Ref<Scene> _scene;
	};
}