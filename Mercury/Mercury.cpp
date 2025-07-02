#include "Mercury.h"

#include <cassert>
#include <fstream>

namespace mercury
{
	static uint8_t ReadUByte(std::ifstream& file)
	{
		uint8_t value = 0;
		file.read((char*)&value, 1);
		return value;
	}

	static uint32_t ReadUInt(std::ifstream& file, bool bSwapEndian)
	{
		uint8_t value[4] = {};
		file.read((char*)&value, 4);

		if (bSwapEndian)
			return (value[0] << 24) | (value[1] << 16) | (value[2] << 8) | value[3];
		else
			return (value[3] << 24) | (value[2] << 16) | (value[1] << 8) | value[0];
	}

	static bool OpenMnistFile(const char* path, uint32_t magicNum, std::ifstream& ifstream)
	{
		if (path == nullptr) 
			return false;

		ifstream.open(path, std::ios::binary);

		if(!ifstream.is_open())
			return false;

		auto val = ReadUInt(ifstream, true);
		if (magicNum != val)
			return false;

		return true;
	}

	mnist loadMNIST(const char* imagePath, const char* labelPath)
	{
		mnist container;

		std::ifstream imageFile;
		if (OpenMnistFile(imagePath, 2051, imageFile))
		{
			auto count = ReadUInt(imageFile, true);
			container.dataset.resize(count);

			container.imageHeight = ReadUInt(imageFile, true);
			container.imageWidth = ReadUInt(imageFile, true);

			auto pixelCount = container.imageHeight * container.imageWidth;

			for (uint32_t Iter = 0; Iter < count; Iter++)
			{
				auto& data = container.dataset[Iter].data;
				data.resize(pixelCount);

				for (uint32_t pixel = 0; pixel < pixelCount; pixel++)
				{
					data[pixel] = ReadUByte(imageFile);
				}
			}
		}

		std::ifstream labelFile;
		if (OpenMnistFile(labelPath, 2049, labelFile))
		{
			auto count = ReadUInt(labelFile, true);

			assert(count == (uint32_t)container.dataset.size());

			container.bHasLabels = count > 0;

			for (uint32_t Iter = 0; Iter < count; Iter++)
			{
				container.dataset[Iter].label = ReadUByte(labelFile);
			}
		}

		return container;
	}
}