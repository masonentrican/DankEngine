#include "dankpch.h"
#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Dank {

	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		//--------------------------------------------------------------
		//			Create Vertex Shader from Source Reference
		//--------------------------------------------------------------

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to OpenGL
		// std::String's .c_str Is NULL character terminated.
		const GLchar* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile vertex shader
		glCompileShader(vertexShader);

		// Check if the compile worked
		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);

		// If we didn't compile upon attempt, destroy shader and return error
		if (isCompiled == GL_FALSE)
		{
			GLint logMaxLength = 0;

			// Get the number of characters in the information log for the shader
			// Is null char terminated
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logMaxLength);

			// Create a vector of GLchars to hold the shader log.
			// Max length includes the NULL character
			std::vector<GLchar> infoLog(logMaxLength);
			glGetShaderInfoLog(vertexShader, logMaxLength, &logMaxLength, &infoLog[0]);

			// Delete the shader before we return the error
			glDeleteShader(vertexShader);

			// Return the DANK error.
			DANK_CORE_ERROR("{0}", infoLog.data());
			DANK_CORE_ASSERT(false, "Vertex shader compilation failed.");
			return;
		}





		//--------------------------------------------------------------
		//			Create Fragment Shader from Source Reference
		//--------------------------------------------------------------

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send fragment shader source code to OpenGL
		// std::String's .c_str Is NULL character terminated.
		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile fragment shader
		glCompileShader(fragmentShader);

		// Check if the compile worked
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);

		// If we didn't compile upon attempt, destroy shader and return error
		if (isCompiled == GL_FALSE)
		{
			GLint logMaxLength = 0;

			// Get the number of characters in the information log for the shader
			// Is null char terminated
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logMaxLength);

			// Create a vector of GLchars to hold the shader log.
			// Max length includes the NULL character
			std::vector<GLchar> infoLog(logMaxLength);
			glGetShaderInfoLog(fragmentShader, logMaxLength, &logMaxLength, &infoLog[0]);

			// Delete the shader before we return the error
			glDeleteShader(fragmentShader);

			// Return the DANK error.
			DANK_CORE_ERROR("{0}", infoLog.data());
			DANK_CORE_ASSERT(false, "Fragment shader compilation failed.");
			return;
		}


		//--------------------------------------------------------------
		//				Link the shaders to OpenGL program
		//--------------------------------------------------------------


		// Link the successfully compiled Vertex and Fragment shaders
		// into the OpenGl program
		_rendererId = glCreateProgram();
		GLuint program = _rendererId;

		// Attach shader to program
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		// Link the program
		glLinkProgram(program);

		// Check if program linked
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);

		// If program is not linked, delete programs and shaders then return error
		if (isLinked == GL_FALSE)
		{
			GLint logMaxLength = 0;

			// Get the number of characters in the information log for the program
			// Is null char terminated
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logMaxLength);

			// Create a vector of GLchars to hold the program log.
			// Max length includes the NULL character
			std::vector<GLchar> infoLog(logMaxLength);
			glGetProgramInfoLog(program, logMaxLength, &logMaxLength, &infoLog[0]);

			// Delete the program
			glDeleteProgram(program);

			// Delete the shaders
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Return Errors
			DANK_CORE_ERROR("{0}", infoLog.data());
			DANK_CORE_ASSERT(false, "Shader link failed.");
			return;
		}

		// Shaders must be detached after linking.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);

	}

	Shader::~Shader()
	{
		glDeleteProgram(_rendererId);
	}

	void Shader::Bind() const
	{
		glUseProgram(_rendererId);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(_rendererId, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

}