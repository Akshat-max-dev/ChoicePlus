#pragma once
#include"c+pch.h"

#include<glad/glad.h>
#include<glm/glm.hpp>

namespace ChoicePlus
{
	class Shader
	{
	public:
		Shader(const std::string& shader);
		~Shader() { glDeleteProgram(mRendererId); }

		void Bind()const { glUseProgram(mRendererId); }
		void UnBind()const { glUseProgram(0); }

		void Int(const std::string& name, int value);
		void Float(const std::string& name, float value);
		void Float2(const std::string& name, const glm::vec2& value);
		void Float3(const std::string& name, const glm::vec3& value);
		void Float4(const std::string& name, const glm::vec4& value);
		void Mat3(const std::string& name, const glm::mat3& value);
		void Mat4(const std::string& name, const glm::mat4& value);
	private:
		const std::map<GLenum, std::string> ReadShader(const std::string& fromFile);
		void CompileShader(const std::map<GLenum, std::string>& data);
	private:
		uint32_t mRendererId;
		std::string mShaderFilename;
	};
}