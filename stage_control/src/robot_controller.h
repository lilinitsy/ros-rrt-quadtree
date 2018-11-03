#include <ros/ros.h>

#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>

#include "laser_reader.h"

class RobotController
{
	public:
		ros::NodeHandle node_handle;
		ros::Publisher velocity_publisher;
		ros::Subscriber laser_scan_subscriber;

		RobotController();

	private:
		// this should not be like this... hmmm
		void laser_scan_callback(LaserReader &laser_reader);
};