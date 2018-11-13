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

	construct_map(image);
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


void ReadMapModule::construct_map(cv::Mat image)
{
	for(int i = 0; i < image.cols; i++)
	{
		for(int j = 0; j < image.rows; j++)
		{
			cv::Scalar colour = image.at<uchar>(cv::Point(i, j));
			map[i][j].pixel_space = cv::Vec2i(i, j);
			map[i][j].world_space = get_world_coordinates(i, j, 28.809, image.cols, image.rows);

			// if the colour is blackish
			if(colour[0] < 50 && colour[1] < 50 && colour[2] < 50)
			{
				map[i][j].blocked = true;
				std::cout << "Blocked at (" << i << ", " << j << ")" << std::endl;
			}
		}
	}

}


cv::Vec2f ReadMapModule::get_world_coordinates(int x, int y, float pixels_to_meters, int pixel_cols, int pixel_rows)
{
	float world_min_x = -30.0f;
	float world_min_y = -30.0f;
	float world_x = ((float) x / (0.5f * (float) pixel_cols)) * pixels_to_meters + world_min_x;
	float world_y = ((float) y / (0.5f * (float) pixel_rows)) * pixels_to_meters + world_min_y;
	return cv::Vec2f(world_x, world_y);
}


cv::Vec2i ReadMapModule::get_pixel_coordinates(float world_x, float world_y, int pixel_rows, int pixel_cols, float pixels_to_meters)
{
	float real_world_min_x = -30.0f;
	float real_world_min_y = -30.0f;

	int pixel_x = 0.5f * pixel_cols / pixels_to_meters * (world_x - real_world_min_x);
	int pixel_y = 0.5f * pixel_rows / pixels_to_meters * (world_y - real_world_min_y);

	return cv::Vec2i(pixel_x, pixel_y);
}