#include "Shader.h"

#include"src/Log.h"

namespace ChoicePlus
{
	static GLenum GetTypeFromString(const std::string& t)
	{
		if (t == "vertex")return GL_VERTEX_SHADER;
		else if (t == "fragment")return GL_FRAGMENT_SHADER;
		else cpassert(0); return GL_NONE;
	}

	Shader::Shader(const std::string& shader)
	{
		auto sources = ReadShader(shader);
		CompileShader(sources);
	}

	const std::map<GLenum, std::string> Shader::ReadShader(const std::string& fromFile)
	{
		std::ifstream file;
		file.open(fromFile, std::ios::in);
		cpassert(!file.fail());
		cpassert(!(file.peek() == EOF));

		std::map<GLenum, std::string> sources;
		std::string line;
		std::stringstream ss;
		GLenum reading_current;

		std::string toFind = "#source";
		size_t sizetoFind = toFind.size();

		while (getline(file, line))
		{
			if (line.find(toFind) != std::string::npos)
			{
				reading_current = GetTypeFromString(line.substr(sizetoFind + 1, line.size() - 1));
				sources.insert({ reading_current, "" });
			}
			else
			{
				auto it = sources.find(reading_current);
				line.append("\n");
				it->second += line;
			}
		}
		
		return sources;
	}

	void Shader::CompileShader(const std::map<GLenum, std::string>& data)
	{
		uint32_t program = glCreateProgram();
		std::vector<uint32_t> shaderIds(data.size());
		int shadercount = 0;
		for (auto& shader : data)
		{
			uint32_t Id = glCreateShader(shader.first);
			const char* src = shader.second.c_str();
			glShaderSource(Id, 1, &src, 0);
			glCompileShader(Id);
			
			int isCompiled;
			glGetShaderiv(Id, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				int maxLength = 0;
				glGetShaderiv(Id, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<char> infoLog(maxLength);
				glGetShaderInfoLog(Id, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(Id);

				std::string msg = infoLog.data();
				msg.append("{w}");
				CONSOLE(msg.c_str());
				CONSOLE("Shader Compilation Failed{e}");
				return;
			}

			glAttachShader(program, Id);

			shaderIds[shadercount++] = Id;
		}

		mRendererId = program;

		glLinkProgram(mRendererId);

		int isLinked;
		glGetProgramiv(mRendererId, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			int maxLength = 0;
			glGetProgramiv(mRendererId, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(mRendererId, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(mRendererId);

			for (auto& shaderId : shaderIds)
			{
				glDetachShader(mRendererId, shaderId);
				glDeleteShader(shaderId);
			}

			std::string msg = infoLog.data();
			msg.append("{w}");
			CONSOLE(msg.c_str());
			CONSOLE("Program Linking Failed");
			cpassert(0);
		}

		for (auto& shaderId : shaderIds)
		{
			glDetachShader(mRendererId, shaderId);
			glDeleteShader(shaderId);
		}

	}
}