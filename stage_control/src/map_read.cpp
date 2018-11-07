#include <iostream>

#include "map_read.h"


ReadMapModule::ReadMapModule()
{
	// nothing to do
}

ReadMapModule::ReadMapModule(std::string map_path)
{
	cv::Mat image = read_map(map_path);
	
	map.map = new int*[image.cols];
	for(int i = 0; i < image.cols; i++)
	{
		map.map[i] = new int[image.rows];
	}

	construct_map(image);
}


cv::Mat ReadMapModule::read_map(std::string map_path)
{
	cv::Mat image = cv::imread(map_path, cv::IMREAD_COLOR);

	if(image.empty())
	{
		printf("Empty map\n");
		std::cout << "PATH: " << map_path << std::endl;
	}

	return image;
}


void ReadMapModule::construct_map(cv::Mat image)
{
	printf("i: %d, j: %d\n", image.cols, image.rows);
	for(int i = 0; i < image.cols; i++)
	{
		for(int j = 0; j < image.rows; j++)
		{
			cv::Scalar colour = image.at<uchar>(cv::Point(i, j));
			if(colour[0] != 255)
				printf("colour at (%d, %d): (%f, %f, %f)\n", i, j, colour[0], colour[1], colour[2]);
			// if colour is black
			if(colour[0] && colour[1] && colour[2] < 50)
			{
				printf("Colour at (%d, %d) is black\n", i, j);
			}
		}
	}
}