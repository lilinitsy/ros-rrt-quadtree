#ifndef MAP_READ_H
#define MAP_READ_H


#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "obstacle.h"


// 2D map structure
// stores x and y
// assign 1 if it's a valid point, 0 if not.
struct Map
{
	cv::Vec2i pixel_space;
	cv::Vec2f world_space;
	bool blocked;
};


class ReadMapModule
{
	public:
		Map **map;
		cv::Vec2i max_pixels;

		ReadMapModule();
		ReadMapModule(std::string map_path, float world_max_x, float world_max_y, float pixels_to_meters, std::vector<Obstacle> obstacles); // don't fuck with 3D yeeeeeeet

	private:
		cv::Mat read_map(std::string map_path);
		cv::Vec2f get_world_coordinate(int x, int y, float world_max_x, float world_max_y, int pixel_rows, int pixel_cols, float pixels_to_meters);
		void construct_map(cv::Mat image, float world_max_x, float world_max_y, float pixels_to_meters, std::vector<Obstacle> obstacles);
};


#endif