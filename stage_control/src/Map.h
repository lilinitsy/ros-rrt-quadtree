#ifndef MAP_READ_H
#define MAP_READ_H

#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


struct Map
{
	cv::Vec2i pixel_space;
	cv::Vec2f world_space;
	bool blocked = false;
};


class ReadMapModule
{
	public:
		Map **map;
		cv::Vec2i max_pixels = cv::Vec2i(0, 0);

		ReadMapModule();
		ReadMapModule(std::string path); // don't fuck with 3D yeeeeeeet
	//	cv::Vec2i get_pixel_coordinates(float world_x, float world_y, float world_max_x, float world_max_y, int pixel_rows, int pixel_cols, float pixels_to_meters);
		cv::Vec2f get_world_coordinates(int x, int y, float pixels_to_meters, int pixel_cols, int pixel_rows);
		cv::Vec2i get_pixel_coordinates(float world_x, float world_y, int pixel_rows, int pixel_cols, float pixels_to_meters);

	private:
		cv::Mat read_map(std::string path);

	//	cv::Vec2f get_world_coordinate(int x, int y, float world_max_x, float world_max_y, int pixel_rows, int pixel_cols, float pixels_to_meters);
		void construct_map(cv::Mat image); // ignore blocks for now
};


#endif