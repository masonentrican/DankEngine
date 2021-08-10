#include "dankpch.h"
#include "LightSource.h"
#include <glad/glad.h>

namespace Dank {

	LightSource::LightSource(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : _ambient(ambient), _diffuse(diffuse), _specular(specular)
	{
		Ref<VertexArray>& vertexArray = VertexArray::Create();
		vertexArray->Bind();
		Ref<VertexBuffer>& vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		vertexBuffer->SetVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		vertexBuffer->SetVertexAttribute(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		_VA = vertexArray;
		_VB = vertexBuffer;
		vertexArray->Unbind();
		_position = glm::vec3(1.2f, 1.0f, 2.0f);
		
	}


	void LightSource::SetPosition(glm::vec3 pos)
	{
		_position = pos;
	}
}