#include "dankpch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Dank {

	//-------------------------------------------------
	//					Vertex Buffer
	//-------------------------------------------------
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		DANK_PROFILE_FUNCTION();

		glCreateBuffers(1, &_rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		DANK_PROFILE_FUNCTION();

		glDeleteBuffers(1, &_rendererId);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		DANK_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		DANK_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//-------------------------------------------------
	//					Index Buffer
	//-------------------------------------------------

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : _count(count)
	{
		DANK_PROFILE_FUNCTION();

		glCreateBuffers(1, &_rendererId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		DANK_PROFILE_FUNCTION();

		glDeleteBuffers(1, &_rendererId);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		DANK_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererId);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		DANK_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}