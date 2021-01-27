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

		/*if (generateMips && srcMipSet.m_nMipLevels == 1)
		{
			int maxMipLevel = CMP_CalcMaxMipLevel(srcMipSet.m_nHeight, srcMipSet.m_nWidth, false);
			int minSize = CMP_CalcMinMipSize(srcMipSet.m_nHeight, srcMipSet.m_nWidth, maxMipLevel + 1);
			cmp_status = CMP_GenerateMIPLevels(&srcMipSet, minSize);
			if (cmp_status != CMP_OK)
			{
				std::string msg = "Failed to load mips for texture " + srcFile;
				msg.append("{e}");
				CONSOLE(msg.c_str());
				return;
			}
		}*/

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

}