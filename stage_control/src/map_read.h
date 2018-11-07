#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


class ReadMapModule
{
	public:
		cv::Mat image;

		ReadMapModule();
		ReadMapModule(std::string map_path);

	private:
		void read_map(std::string map_path);
		void construct_map();
};