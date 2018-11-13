#include <iostream>
#include <sstream>

#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Point32.h>
#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/Twist.h>


#include "Robot.h"


Robot::Robot()
{
	velocity_publisher = node_handle.advertise<geometry_msgs::Twist>("cmd_vel", 1); // TODO: Look into this more
	pose_subscriber = node_handle.subscribe<nav_msgs::Odometry>("odom", 60, &Robot::pose_callback, this);
	map = ReadMapModule("src/ros-rrt-quadtree/bitmaps/autolab.png");
}


void Robot::pose_callback(const nav_msgs::Odometry::ConstPtr &o)
{
	pose.orientation = o->pose.pose.orientation;
	pose.position = o->pose.pose.position;
}


void Robot::run()
{
	ros::Rate rate(1);
	cv::Vec2i position = cv::Vec2i(pose.position.x, pose.position.y);
	cv::Vec2i pixel_position = map.get_pixel_coordinates(pose.position.x, pose.position.y, 689, 809, 28.808f);
	cv::Vec2i goal_position = cv::Vec2i(586, 458);

	printf("pixel_position: %d %d\n", pixel_position.val[0], pixel_position.val[1]);
	rrt = RRT(goal_position, 3); // step size 3 pixels
	rrt.build_rrt(pixel_position, map, 10);
	printf("RRT built\n");
	printf("RRT nodes size: %lu\n", rrt.nodes.size());
	while(ros::ok())
	{
		ros::spinOnce();
	}
}