#include <ros/ros.h>

#include "Map.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "robot_controller");
	ReadMapModule map = ReadMapModule("src/ros-rrt-quadtree/bitmaps/autolab.png");
	return 0;
}