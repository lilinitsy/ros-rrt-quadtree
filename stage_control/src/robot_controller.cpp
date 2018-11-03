#include <iostream>
#include <sstream>

#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Twist.h>


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
	pose_subscriber = node_handle.subscribe<nav_msgs::Odometry>("base_pose_ground_truth", 60, &RobotController::pose_callback, this);
	laser_reader = LaserReader();
}


void RobotController::pose_callback(const nav_msgs::Odometry::ConstPtr &o)
{
	pose.orientation = o->pose.pose.orientation;
	pose.position = o->pose.pose.position;
	ROS_INFO_STREAM("IN THE POSE CALLBACK, SOMETHING ISN'T WORKING");
	ROS_INFO_STREAM("Pose orientation: " << pose.orientation);
	ROS_INFO_STREAM("Pose position: " << pose.position);
	ROS_INFO_STREAM("Pose orientation pointer: " << o->pose.pose.orientation);
	ROS_INFO_STREAM("Pose position pointer: " << o->pose.pose.position);
}


void RobotController::print_pose()
{
//	ROS_INFO_STREAM("Pose orientation: " << pose.orientation);
//	ROS_INFO_STREAM("Pose position: " << pose.position);
}

void RobotController::run()
{
	ros::Rate rate(10);
	while(ros::ok())
	{
		ros::spinOnce();
		//laser_reader.scan_world();
		print_pose();
	}
}