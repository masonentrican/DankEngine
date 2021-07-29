#include "dankpch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Dank {

	//-------------------------------------------------
	//					Vertex Buffer
	//-------------------------------------------------
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &_rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &_rendererId);
	}

	void OpenGLVertexBuffer::SetVertexAttribute(int index, int size, int type, unsigned char normalized, unsigned long long stride, const void* offset)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, size, type, normalized, (GLsizei)stride, offset);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//-------------------------------------------------
	//					Index Buffer
	//-------------------------------------------------

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : _count(count)
	{
		glCreateBuffers(1, &_rendererId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &_rendererId);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererId);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}