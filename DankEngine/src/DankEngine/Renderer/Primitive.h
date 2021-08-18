#pragma once
#include "dankpch.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Dank {
	class Primitive
	{
	public:
		std::vector<float> _vertices;
		float _size;
		glm::vec3 _color;
		glm::vec3 _position;

		Primitive(std::vector<float>& vertices);

	private:

	};
}

