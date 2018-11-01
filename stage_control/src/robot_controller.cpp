#include <iostream>
#include <sstream>


#include <geometry_msgs/Twist.h>
#include <ros/ros.h>
#include <std_msgs/String.h>


/* 	robot prolly has some kind of sensor
		->	sensor detects obstacles
			-> So robot uh, subscribes to it, mayyybeeee333e
		-> robot then calls some quadtree shit
		-> RRT on the quadtreeeee maybe I should review the notes

*/

int main(int argc, char **argv)
{
	ros::init(argc, argv, "robot_controller");
	ros::NodeHandle node_handle;
	ros::Publisher velocity_publisher = node_handle.advertise<geometry_msgs::Twist>("/cmd_vel", 1); // TODO: Look into this more

	// send twist commands
	geometry_msgs::Twist base_cmd;
	ros::Rate loop_rate(60);

	while(ros::ok())
	{
		// TODO: Kinematics
		
	}

	
	

	return 0;
}