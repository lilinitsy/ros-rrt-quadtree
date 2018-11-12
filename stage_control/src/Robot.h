#ifndef ROBOT_H
#define ROBOT_H


#include <vector>

#include <ros/ros.h>

#include <geometry_msgs/Point.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/String.h>


#include "Map.h"
#include "RRTNode.h"
#include "RRT.h"


class Robot
{
	public:
		ros::NodeHandle node_handle;
		ros::Publisher velocity_publisher;
		ros::Subscriber pose_subscriber;
		geometry_msgs::Pose pose;

		RRT rrt;
		ReadMapModule map;

		Robot();
		void run();

	private:
		void pose_callback(const nav_msgs::Odometry::ConstPtr &o);
		void print_pose();
};



#endif