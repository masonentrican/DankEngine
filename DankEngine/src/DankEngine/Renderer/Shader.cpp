#include "dankpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Dank {

	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		DANK_PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    DANK_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLShader>(filepath);
		}

		DANK_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		DANK_PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    DANK_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		DANK_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		DANK_PROFILE_FUNCTION();

		DANK_CORE_ASSERT(!Exists(name), "Shader already exists!");
		_shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		DANK_PROFILE_FUNCTION();

		auto& name = shader->GetName();
		Add(name, shader);
	}

	Dank::Ref<Dank::Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		DANK_PROFILE_FUNCTION();

		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Dank::Ref<Dank::Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		DANK_PROFILE_FUNCTION();

		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Dank::Ref<Dank::Shader> ShaderLibrary::Get(const std::string& name)
	{
		DANK_PROFILE_FUNCTION();

		DANK_CORE_ASSERT(Exists(name), "Shader not found!");
		return _shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		DANK_PROFILE_FUNCTION();

		return _shaders.find(name) != _shaders.end();
	}

}