#pragma once

#include "DankEngine/Renderer/Buffer.h"

//------------------------------------------------
// Interface for OpenGL vertex and index buffers
//------------------------------------------------

namespace Dank {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

	private:
		uint32_t _rendererId;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual uint32_t GetCount() const { return _count; }
	private:
		uint32_t _rendererId;
		uint32_t _count;
	};
}