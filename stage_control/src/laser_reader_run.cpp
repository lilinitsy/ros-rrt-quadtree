#include "laser_reader.h"

// probably obsolete

int main(int argc, char **argv)
{
	ros::init(argc, argv, "laser_reader");

	LaserReader laser_reader = LaserReader();
	laser_reader.scan_world();

	return 0;
}