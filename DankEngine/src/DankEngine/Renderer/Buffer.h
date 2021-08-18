#pragma once

namespace Dank {

	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case Dank::ShaderDataType::Float:		return 4; 
			case Dank::ShaderDataType::Float2:		return 4 * 2; 
			case Dank::ShaderDataType::Float3:		return 4 * 3; 
			case Dank::ShaderDataType::Float4:		return 4 * 4; 
			case Dank::ShaderDataType::Mat3:		return 4 * 3 * 3;
			case Dank::ShaderDataType::Mat4:		return 4 * 4 * 4;
			case Dank::ShaderDataType::Int:			return 4;
			case Dank::ShaderDataType::Int2:		return 4 * 2;
			case Dank::ShaderDataType::Int3:		return 4 * 3;
			case Dank::ShaderDataType::Int4:		return 4 * 4;
			case Dank::ShaderDataType::Bool:		return 1;			
		}

		DANK_CORE_ASSERT(false, "Unknown ShaderDataType when calculating size");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		BufferElement() {}

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false) : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case Dank::ShaderDataType::Float:		return 1;
				case Dank::ShaderDataType::Float2:		return 2;
				case Dank::ShaderDataType::Float3:		return 3;
				case Dank::ShaderDataType::Float4:		return 4;
				case Dank::ShaderDataType::Mat3:		return 3 * 3;
				case Dank::ShaderDataType::Mat4:		return 4 * 4;
				case Dank::ShaderDataType::Int:			return 1;
				case Dank::ShaderDataType::Int2:		return 2;
				case Dank::ShaderDataType::Int3:		return 3;
				case Dank::ShaderDataType::Int4:		return 4;
				case Dank::ShaderDataType::Bool:		return 1;
			}

			DANK_CORE_ASSERT(false, "Unknown ShaderDataType in get component count");
			return 0;
		}
	};

	// Defines a vertex buffer layout. Contains a vector of buffer elements
	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& elements) : _elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		inline uint32_t GetStride() const { return _stride; }
		inline const std::vector<BufferElement>& GetElements() const { return _elements; }

		std::vector<BufferElement>::iterator begin() { return _elements.begin(); }
		std::vector<BufferElement>::iterator end() { return _elements.end(); }

		std::vector<BufferElement>::const_iterator begin() const { return _elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return _elements.end(); }

	private:
		void CalculateOffsetsAndStride()
		{
			uint32_t offset = 0;
			_stride = 0;

			for (auto& element : _elements)
			{
				element.Offset = offset;
				offset += element.Size;
				_stride += element.Size;
			}
		}
	private:
		std::vector<BufferElement> _elements;
		uint32_t _stride = 0;
	};
	
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;
		virtual void SetData(const void* data, uint32_t offset, uint32_t size) = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
		static Ref<VertexBuffer> Create(uint32_t size);
		virtual void SetVertexAttribute(int index, int size, int type, unsigned char normalized, unsigned long long stride, const void* offset) = 0;
		
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	};

}