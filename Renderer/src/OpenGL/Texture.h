#pragma once
#include"c+pch.h"

#include<glad/glad.h>
#include<gli/gli.hpp>

#include"src/Log.h"

namespace ChoicePlus
{
	enum class BlockCompressionFormat
	{
		BC1 = 44, BC2 = 45, BC3 = 46, BC4 = 47, BC5 = 49, BC6 = 51, BC7 = 53
	};

	const std::string CompressTexture(const std::string& srcFile, std::string& dstDirectory,
		BlockCompressionFormat format, bool generateMips);

	enum class TextureTypes
	{
		TWO_D = 0, CUBEMAP = 1
	};

	template<TextureTypes T>
	class Texture{};

	template<>
	class Texture<TextureTypes::TWO_D>
	{
	public:
		Texture(const std::string& srcCompressed);

		const std::string& GetFilepath()const { return mSrcCompressed; }

		void Bind(uint32_t slot)const { glActiveTexture(GL_TEXTURE0 + slot); glBindTexture(GL_TEXTURE_2D, mRendererId); }
	private:
		void Load();
	private:
		std::string mSrcCompressed;
		uint32_t mRendererId;
	};

	inline Texture<TextureTypes::TWO_D>::Texture(const std::string& srcCompressed) 
		:mSrcCompressed(srcCompressed)
	{
		Load();
	}

	inline void Texture<TextureTypes::TWO_D>::Load()
	{
		gli::texture _texture = gli::load(mSrcCompressed.c_str());

		if (_texture.empty())
		{
			if (!mSrcCompressed.empty())
			{
				std::string msg = "Failed to load compressed texture " + mSrcCompressed;
				msg.append("{e}");
				CONSOLE(msg.c_str());
			}
			return;
		}

		gli::gl _gl(gli::gl::profile::PROFILE_GL33);
		gli::gl::format _format = _gl.translate(_texture.format(), _texture.swizzles());

		GLenum _target = _gl.translate(_texture.target());
		if (!(gli::is_compressed(_texture.format()) && _texture.target() == gli::TARGET_2D))
		{
			std::string msg = "Texture " + mSrcCompressed + " ";
			msg += "is either not 2D or is not compressed{w}";
			CONSOLE(msg.c_str());
			return;
		}

		glCreateTextures(_target, 1, &mRendererId);
		glBindTexture(_target, mRendererId);

		glTexParameteri(_target, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(_target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(_texture.levels() - 1));
		glTexParameteriv(_target, GL_TEXTURE_SWIZZLE_RGBA, &_format.Swizzles[0]);

		glTexStorage2D(_target, static_cast<GLint>(_texture.levels()), _format.Internal, _texture.extent().x, _texture.extent().y);
		for (size_t Level = 0; Level < _texture.levels(); ++Level)
		{
			glm::tvec3<GLsizei> Extent(_texture.extent(Level));
			glCompressedTexSubImage2D(
				_target, static_cast<GLint>(Level), 0, 0, Extent.x, Extent.y,
				_format.Internal, static_cast<GLsizei>(_texture.size(Level)), _texture.data(0, 0, Level));
		}
	}
}