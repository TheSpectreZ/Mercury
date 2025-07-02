#pragma once
#include <vector>

namespace mercury
{
	template<typename DataType, typename LabelType>
	struct datasample
	{
		LabelType label = LabelType{ 0 };
		std::vector<DataType> data;
	};

	template<typename DataType, typename LabelType>
	using dataset = std::vector<datasample<DataType, LabelType>>;
	
	//////////////////////

	struct mnist
	{
		dataset<uint8_t, uint8_t> dataset;
		
		bool bHasLabels = false;
		uint32_t imageWidth = 0;
		uint32_t imageHeight = 0; 
	};


	mnist loadMNIST(const char* imagePath, const char* labelPath);
}
