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
	velocity_publisher = node_handle.advertise<geometry_msgs::Twist>("/cmd_vel", 1); // TODO: Look into this more
//	laser_scan_subscriber = node_handle.subscribe<sensor_msgs::LaserScan>("laser_scan", 60, &RobotController::laser_scan_callback, this);

}


void RobotController::laser_scan_callback(LaserReader &laser_reader)
{

}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "robot_controller");
	ros::NodeHandle node_handle;

	// send twist commands
	geometry_msgs::Twist base_cmd;
	ros::Rate loop_rate(60);

	while(ros::ok())
	{
		// TODO: Kinematics
		
	}

	
	

	return 0;
}