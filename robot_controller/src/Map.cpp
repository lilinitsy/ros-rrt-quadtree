#include <iostream>

#include "Map.h"



ReadMapModule::ReadMapModule()
{
	// nuttin to do
}


ReadMapModule::ReadMapModule(std::string path)
{
	cv::Mat image = read_map(path);
	max_pixels = cv::Vec2i(image.cols, image.rows);

	map = new Map*[image.cols];
	for(int i = 0; i < image.cols; i++)
	{
		map[i] = new Map[image.rows];
	}
}


cv::Mat ReadMapModule::read_map(std::string path)
{
	cv::Mat image = cv::imread(path, cv::IMREAD_COLOR);
	if(image.empty())
	{
		std::cout << "Empty map PATH: " << path << std::endl;
	}

	return image;
}