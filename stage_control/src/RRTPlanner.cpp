#include <ros/ros.h>

#include "Map.h"
#include "Robot.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "robot_controller");
	Robot robot = Robot();
	robot.run();
	return 0;
}