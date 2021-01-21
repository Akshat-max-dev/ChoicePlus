#pragma once
#include"c+pch.h"

#include<glad/glad.h>

namespace ChoicePlus
{
	class Shader
	{
	public:
		Shader(const std::string& shader);
		~Shader() { glDeleteProgram(mRendererId); }

		void Bind()const { glUseProgram(mRendererId); }
		void UnBind()const { glUseProgram(0); }
	private:
		const std::map<GLenum, std::string> ReadShader(const std::string& fromFile);
		void CompileShader(const std::map<GLenum, std::string>& data);
	private:
		uint32_t mRendererId;
	};
}