#include "map_read.h"


ReadMapModule::ReadMapModule()
{
	// nothing to do
}

ReadMapModule::ReadMapModule(std::string map_path)
{
	cv::Mat image = read_map(map_path);
	construct_map();
}


cv::Mat ReadMapModule::read_map(std::string map_path)
{
	cv::Mat image = cv::imread(map_path, cv::IMREAD_GRAYSCALE);

	if(image.empty())
	{
		printf("Empty map\n");
	}

	return image;
}


void ReadMapModule::construct_map(cv::Mat image)
{
	*map = new Map[]
}