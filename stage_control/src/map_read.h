#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


// 2D map structure
// stores x and y
// assign 1 if it's a valid point, 0 if not.
struct Map
{
	int **map;
};


class ReadMapModule
{
	public:
		Map map;

		ReadMapModule();
		ReadMapModule(std::string map_path);

	private:
		cv::Mat read_map(std::string map_path);
		void construct_map();
};