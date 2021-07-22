#include "dankpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Dank {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Dank::ShaderDataType::Float:    return GL_FLOAT;
		case Dank::ShaderDataType::Float2:   return GL_FLOAT;
		case Dank::ShaderDataType::Float3:   return GL_FLOAT;
		case Dank::ShaderDataType::Float4:   return GL_FLOAT;
		case Dank::ShaderDataType::Mat3:     return GL_FLOAT;
		case Dank::ShaderDataType::Mat4:     return GL_FLOAT;
		case Dank::ShaderDataType::Int:      return GL_INT;
		case Dank::ShaderDataType::Int2:     return GL_INT;
		case Dank::ShaderDataType::Int3:     return GL_INT;
		case Dank::ShaderDataType::Int4:     return GL_INT;
		case Dank::ShaderDataType::Bool:     return GL_BOOL;
		}

		DANK_CORE_ASSERT(false, "Unknown ShaderDataType in OpenGL base type conversion");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		DANK_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &_rendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		DANK_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &_rendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		DANK_PROFILE_FUNCTION();

		glBindVertexArray(_rendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		DANK_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		DANK_PROFILE_FUNCTION();

		DANK_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(_rendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}

		_vertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		DANK_PROFILE_FUNCTION();

		glBindVertexArray(_rendererID);
		indexBuffer->Bind();

		_indexBuffer = indexBuffer;
	}

}