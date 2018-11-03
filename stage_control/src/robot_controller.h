#include <ros/ros.h>

#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>

#include "laser_reader.h"

class RobotController
{
	public:
		ros::NodeHandle node_handle;
		ros::Publisher velocity_publisher;
		LaserReader laser_reader;

		RobotController();
		void run();

	private:
};