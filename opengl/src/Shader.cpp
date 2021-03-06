
#include"Shader.h"
#include<fstream>
#include<sstream>
#include<iostream>
#include<glad/glad.h>

#include<glm/gtc/type_ptr.hpp>
namespace OGL
{
	Shader::Shader(const std::string& filePath)
	{
		std::pair<std::string, std::string> sources = ReadFile(filePath);
		auto& vertexSouce = sources.first;
		auto& fragmentSouce = sources.second;
		uint32_t vertexShaderId = compileShader(vertexSouce, 0);
		uint32_t fragmentShaderId = compileShader(fragmentSouce, 1);

		if (vertexShaderId == -1 || fragmentShaderId == -1) assert(false);
		m_ProgramId = glCreateProgram();
		glAttachShader(m_ProgramId, vertexShaderId);
		glAttachShader(m_ProgramId, fragmentShaderId);
		
		glLinkProgram(m_ProgramId);
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);
	}
	Shader::~Shader()
	{
		glDeleteProgram(m_ProgramId);
	}
	void Shader::Bind() const
	{
		glUseProgram(m_ProgramId);
	}
	void Shader::UnBind() const
	{
		glUseProgram(0);
	}
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		return std::make_shared<Shader>(filepath);
	}



	void Shader::SetMat4(const std::string& name, const glm::mat4& value)
	{
	
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
		
	}

	void Shader::SetFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniform1f(location, value);

	}
	void Shader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
		
	}
	void Shader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	
	}
	void Shader::SetInt1(const std::string& name, int value)
	{
		
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniform1i(location, value);
	
	}
	void Shader::SetIntArray(const std::string& name, int* values, unsigned int count)
	{
	
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniform1iv(location, count, values);
	
	}





	std::pair<std::string, std::string> Shader::ReadFile(const std::string& filePath)
	{
		std::ifstream inFile(filePath);
		if (!inFile.is_open())
			assert(false);

		std::string s;
		std::stringstream shaderSource[2];
		//type=0????????vertexSource;type=1????????fragmentshader
		int type = -1;
		while (!inFile.eof())
		{
			std::getline(inFile, s);
			if (s.find("vertex")!=std::string::npos)
			{
				type = 0;
				continue;
			}
			if (s.find("fragment")!= std::string::npos)
			{
				type = 1;
				continue;
			}
			if (type == -1) return{ shaderSource[0].str(),shaderSource[1].str() };
			shaderSource[type] << s << std::endl;

		}

		return {shaderSource[0].str(),shaderSource[1].str() };
	}
	uint32_t Shader::compileShader(const std::string& sourceCode, int type)
	{
		const char* source = sourceCode.c_str();
		uint32_t shaderId = glCreateShader(type == 0 ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
		glShaderSource(shaderId, 1, &source, nullptr);
		glCompileShader(shaderId);

		int success;
		char infoLog[512];
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
			std::cout << infoLog << std::endl;
			glDeleteShader(shaderId);
			return -1;
		}
		return shaderId;
	}

	void Shader::Reset(const std::string& filename)
	{
		std::pair<std::string, std::string> sources = ReadFile(filename);
		auto& vertexSouce = sources.first;
		auto& fragmentSouce = sources.second;
		uint32_t vertexShaderId = compileShader(vertexSouce, 0);
		uint32_t fragmentShaderId = compileShader(fragmentSouce, 1);

		if (vertexShaderId == -1 || fragmentShaderId == -1)
		{
			return;
		}
		else
		{
			if (m_ProgramId >= 0) glDeleteProgram(m_ProgramId);
		}
		m_ProgramId = glCreateProgram();
		glAttachShader(m_ProgramId, vertexShaderId);
		glAttachShader(m_ProgramId, fragmentShaderId);

		glLinkProgram(m_ProgramId);
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);
	}

	bool Shader::Compile(const std::string& filename)
	{
		std::pair<std::string, std::string> sources = ReadFile(filename);
		auto& vertexSouce = sources.first;
		auto& fragmentSouce = sources.second;
		uint32_t vertexShaderId = compileShader(vertexSouce, 0);
		uint32_t fragmentShaderId = compileShader(fragmentSouce, 1);

		if (vertexShaderId == -1 || fragmentShaderId == -1)
			return false;
		return true;
	}
}