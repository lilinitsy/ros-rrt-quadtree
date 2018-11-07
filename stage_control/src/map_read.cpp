#include "map_read.h"


ReadMapModule::ReadMapModule()
{
	// nothing to do
}

ReadMapModule::ReadMapModule(std::string map_path)
{
	read_map(map_path);
	construct_map();
}


void ReadMapModule::read_map(std::string map_path)
{
	image = cv::imread(map_path, cv::IMREAD_COLOR);

	if(image.empty())
	{

	}
}


void ReadMapModule::construct_map()
{

}