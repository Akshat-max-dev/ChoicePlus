#include"Texture.h"

#include<cmp_framework.h>

namespace ChoicePlus
{
	const std::string CompressTexture(const std::string& srcFile, std::string& dstDirectory, BlockCompressionFormat format, bool generateMips)
	{
		CMP_MipSet srcMipSet = {};
		int cmp_status = CMP_LoadTexture(srcFile.c_str(), &srcMipSet);
		if (cmp_status != CMP_OK)
		{
			std::string msg = "Failed to load texture " + srcFile;
			msg.append("{e}");
			CONSOLE(msg.c_str());
			return {};
		}

		if (generateMips && srcMipSet.m_nMipLevels == 1)
		{
			int minSize = CMP_CalcMinMipSize(srcMipSet.m_nHeight, srcMipSet.m_nWidth, srcMipSet.m_nMaxMipLevels);
			cmp_status = CMP_GenerateMIPLevels(&srcMipSet, minSize);
			if (cmp_status != CMP_OK)
			{
				std::string msg = "Failed to load mips for texture " + srcFile;
				msg.append("{e}");
				CONSOLE(msg.c_str());
				return {};
			}
		}

		KernelOptions kerneloptions = {};
		kerneloptions.format = (CMP_FORMAT)format;
		kerneloptions.fquality = 0.05f;

		CMP_MipSet dstMipSet = {};
		cmp_status = CMP_ProcessTexture(&srcMipSet, &dstMipSet, kerneloptions, nullptr);
		if (cmp_status != CMP_OK)
		{
			std::string msg = "Failed to process texture " + srcFile;
			msg.append("{e}");
			CONSOLE(msg.c_str());
			return {};
		}

		std::string srcRelative = std::filesystem::path(srcFile).stem().string() +
			std::filesystem::path(srcFile).extension().string() + ".dds";
		dstDirectory += srcRelative;

		CMP_SaveTexture(dstDirectory.c_str(), &dstMipSet);

		CMP_FreeMipSet(&srcMipSet);
		CMP_FreeMipSet(&dstMipSet);

		return dstDirectory;
	}

	const uint32_t LoadCompressedTexture2D(const std::string& srcCompressed)
	{

		gli::texture Texture = gli::load(srcCompressed.c_str());
		if (Texture.empty())
		{
			if (!srcCompressed.empty())
			{
				std::string msg = "Failed to load compressed texture " + srcCompressed;
				msg.append("{e}");
				CONSOLE(msg.c_str());
			}
			return 0;
		}

		gli::gl GL(gli::gl::PROFILE_GL33);
		gli::gl::format const Format = GL.translate(Texture.format(), Texture.swizzles());
		GLenum Target = GL.translate(Texture.target());

		if (!(gli::is_compressed(Texture.format())) && Target == gli::TARGET_2D)
		{
			std::string msg = "Texture " + srcCompressed + " ";
			msg += "is either not 2D or is not compressed{w}";
			CONSOLE(msg.c_str());
			return 0;
		}

		GLuint TextureName = 0;
		glCreateTextures(Target, 1, &TextureName);
		glBindTexture(Target, TextureName);
		glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(Target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(Target, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(Target, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(Target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(Texture.levels() - 1));
		glTexParameteriv(Target, GL_TEXTURE_SWIZZLE_RGBA, &Format.Swizzles[0]);
		glTexStorage2D(Target, static_cast<GLint>(Texture.levels()), Format.Internal, 
			Texture.extent().x, Texture.extent().y);
		for (size_t Level = 0; Level < Texture.levels(); ++Level)
		{
			glm::tvec3<GLsizei> Extent(Texture.extent(Level));
			glCompressedTexSubImage2D(
				Target, static_cast<GLint>(Level), 0, 0, Extent.x, Extent.y,
				Format.Internal, static_cast<GLsizei>(Texture.size(Level)), Texture.data(0, 0, Level));
		
			memset(Texture.data(0, 0, Level), 0, Texture.size(Level));
		}

		memset(Texture.data(), 0, Texture.size());

		return TextureName;
	}

}