#pragma once
#include "dankpch.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Dank {
	class Primitive
	{
	public:
		Ref<VertexArray> _VA;
		Ref<VertexBuffer> _VB;
		float _size;
		glm::vec3 _color;
		glm::vec3 _position;

		Primitive(Ref<VertexArray>& VA, Ref<VertexBuffer>& VB);

	private:

	};
}

