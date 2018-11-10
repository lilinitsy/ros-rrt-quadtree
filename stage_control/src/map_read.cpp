#include <iostream>

#include "map_read.h"

ReadMapModule::ReadMapModule()
{
	// nothing to do
}

ReadMapModule::ReadMapModule(std::string map_path, float world_max_x, float world_max_y, float pixels_to_meters, std::vector<Obstacle> obstacles)
{
	cv::Mat image = read_map(map_path);
	max_pixels = cv::Vec2i(image.cols, image.rows);
	
	map = new Map*[image.cols];
	for(int i = 0; i < image.cols; i++)
	{
		map[i] = new Map[image.rows];
	}

	construct_map(image, world_max_x, world_max_y, pixels_to_meters, obstacles);
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


void ReadMapModule::construct_map(cv::Mat image, float world_max_x, float world_max_y, float pixels_to_meters, std::vector<Obstacle> obstacles)
{
	for(int i = 0; i < image.cols; i++)
	{
		for(int j = 0; j < image.rows; j++)
		{
			cv::Scalar colour = image.at<uchar>(cv::Point(i, j));
			map[i][j].pixel_space = cv::Vec2i(i, j);
			map[i][j].world_space = get_world_coordinate(i, j, world_max_x, world_max_y, image.rows, image.cols, pixels_to_meters);
		
			// if colour is blackish
			if(colour[0] < 50 && colour[1] < 50 && colour[2] < 50)
			{
			//	printf("Colour at (%d, %d) is black\n", map[i][j].pixel_space[0], map[i][j].pixel_space[1]);
				map[i][j].blocked = true;
			//	map.map[i, j] = 
			}

			// test for blocks
			// this shit isn't working
			else 
			{
				for(unsigned int k = 0; i < obstacles.size(); i++)
				{
					// check if the world_space coords overlap
					bool x_overlap_left_edge 	= obstacles[k].position.val[0] - obstacles[k].size.val[0] 	<= map[i][j].world_space.val[0];
					bool x_overlap_right_edge 	= obstacles[k].position.val[0] + obstacles[k].size.val[0] 	>= map[i][j].world_space.val[0];
					bool y_overlap_bottom_edge 	= obstacles[k].position.val[1] - obstacles[k].size.val[1] 	<= map[i][j].world_space.val[1];
					bool y_overlap_top_edge 	= obstacles[k].position.val[1] + obstacles[k].size.val[1] 	>= map[i][j].world_space.val[1];
			
					if(x_overlap_left_edge && x_overlap_right_edge && y_overlap_bottom_edge && y_overlap_top_edge)
					{
						printf("TRUE\n");
					}

					if(x_overlap_left_edge)
					{
						printf("%d, %d: x_overlap_left_edge\n", i, j);
					}

					if(x_overlap_right_edge)
					{
						printf("%d, %d: x_overlap_right_edge\n", i, j);
					}

					if(y_overlap_bottom_edge)
					{
						printf("%d, %d: y_overlap_bottom_edge\n", i, j);
					}

					if(y_overlap_top_edge)
					{
						printf("%d, %d: y_overlap_top_edge\n", i, j);
					}
				}
			}
		}
	}

	/*
	for(int i = 0; i < image.cols; i++)
	{
		for(int j = 0; j < image.rows; j++)
		{
			printf("pixel space: (%d, %d)\tworld space: (%f, %f)\n", map[i][j].pixel_space[0], map[i][j].pixel_space[1], map[i][j].world_space[0], map[i][j].world_space[1]);
		}
	}


	std::cout << "px, py: " << "5" << " " << "5" << "\tworld_x, world_y: " << map[5][5].world_space.val[0] << " " << map[5][5].world_space.val[1] << std::endl;
	*/
}


// probably going to plan path in image (ooh,a nd need to add blocks as obstacles too, in the construct_map
// loop), and then ... oh but maybe cause I'm doing the conversionn, don't need to
// can just do the conversions, then plan in stage, and convert back to check if valid? Will need to test
cv::Vec2f ReadMapModule::get_world_coordinate(int x, int y, float world_max_x, float world_max_y, int pixel_rows, int pixel_cols, float pixels_to_meters)
{
	// origin points
	// TODO: Pass these as parameters and don't compute in the for loops
	float real_world_max_x = world_max_x / 2.0f;
	float real_world_max_y = world_max_y / 2.0f;
	float real_world_min_x = -1.0f * real_world_max_x;
	float real_world_min_y = -1.0f * real_world_max_y;

	

	// x pixel corresponds to negative world
	float world_x = ((float) x / (0.5f * (float) pixel_cols)) * pixels_to_meters + real_world_min_x;
	float world_y = ((float) y / (0.5f * (float) pixel_rows)) * pixels_to_meters + real_world_min_y;
	
	
	// test case
	if(x == 0 && y == 0)
	{
	//	std::cout << "x, y: " << x << " " << y << "\tworld_x, world_y: " << world_x << " " << world_y << std::endl;
	}
	//printf("pixel: %f %f, world: %f %f\n", x, y, world_x, world_y);

	return cv::Vec2f(world_x, world_y);
}