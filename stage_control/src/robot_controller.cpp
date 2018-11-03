#include <iostream>
#include <sstream>

#include <sensor_msgs/LaserScan.h>


#include "robot_controller.h"
/* 	robot prolly has some kind of sensor
		->	sensor detects obstacles
			-> So robot uh, subscribes to it, mayyybeeee333e
		-> robot then calls some quadtree shit
		-> RRT on the quadtreeeee maybe I should review the notes

*/


RobotController::RobotController()
{
	velocity_publisher = node_handle.advertise<geometry_msgs::Twist>("cmd_vel", 1); // TODO: Look into this more
	laser_reader = LaserReader();
}

void RobotController::run()
{
	ros::Rate rate(10);
	while(ros::ok())
	{
		ros::spinOnce();
		laser_reader.scan_world();
	}
}