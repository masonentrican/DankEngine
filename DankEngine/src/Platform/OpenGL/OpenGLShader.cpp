#include "dankpch.h"
#include "OpenGLShader.h"

#include <fstream>

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Dank {	

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;

		if (type == "fragment")
			return GL_FRAGMENT_SHADER;

		DANK_CORE_ASSERT(false, "Unknown shader type in OpenGLShader type string parsing");
		return 0;
	}

	// Used for supplying a filepath
	OpenGLShader::OpenGLShader(const std::string& filepath)
	{	
		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		
		Compile(shaderSources);

		// Retrieve name of shader
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		_name = filepath.substr(lastSlash, count);
	}

	// Use for supplying src references
	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) : _name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;

		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(_rendererID);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream inputFile(filepath, std::ios::in | std::ios::binary);

		if (inputFile)
		{
			// Goto end of file
			inputFile.seekg(0, std::ios::end);

			// Find the position at the end so we can determine size
			result.resize(inputFile.tellg());

			// Go back to beginning
			inputFile.seekg(0, std::ios::beg);

			// Read the file from beginning with size of result
			inputFile.read(&result[0], result.size());

			inputFile.close();

			return result;
		}
		else
		{
			DANK_CORE_ERROR("OpenGLShader could not open file '{0}'", filepath);
		}
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		// The token to define the type of shader
		const char* typeToken = "#type";				

		size_t typeTokenLength = strlen(typeToken);			
		size_t pos = source.find(typeToken, 0);
		
		// While a token exists
		while (pos != std::string::npos)
		{
			// Find the first instance of a new line or return so we have the position. If a token doesn't exist here
			// that signifies a problem we need to assert
			size_t eol = source.find_first_of("\r\n", pos);
			DANK_CORE_ASSERT(eol != std::string::npos, "Syntax error in OpenGLShader - are there extra whitespaces at the token?");

			// Advance passed the beginning position + the size of the token + 1 for a whitespace
			// so we can grab what type this is
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			DANK_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified in OpenGLShader");

			// Find the next line and look for another type token
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);

			// Search from the next line to the end of the file or next token
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));

		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();
		std::array<GLenum, 2> glShaderIDs;
		DANK_CORE_ASSERT(shaderSources.size() <= 2, "Only 2 shaders per file supported for now");

		int glShaderIDIndex = 0;
		for (auto&& [key,value] : shaderSources)
		{
			GLenum type = key;
			const std::string& source = value;

			GLuint shader = glCreateShader(type);

			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				DANK_CORE_ERROR("{0}", infoLog.data());
				DANK_CORE_ASSERT(false, "Shader compilation failure");
				break;
			} 

			// Attach shader on successful compile
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		// Shaders are successfully compiled.
		// Now time to link them together into a program.
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			//glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			// Don't leak shaders either.
			for (auto id : glShaderIDs)			
				glDeleteShader(id);		

			DANK_CORE_ERROR("{0}", infoLog.data());
			DANK_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShaderIDs)
			glDetachShader(program, id);

		_rendererID = program;
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(_rendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		UploadUniformMat4(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec4& value)
	{
		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(_rendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(_rendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(_rendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(_rendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(_rendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(_rendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(_rendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}