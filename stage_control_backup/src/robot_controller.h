#ifndef ROBOT_CONTROLLER_H
#define ROBOT_CONTROLLER_H

#include <vector>

#include <ros/ros.h>

#include <geometry_msgs/Point.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/String.h>

#include "laser_reader.h"
#include "map_read.h"
#include "obstacle.h"
#include "rrt.h"

class RobotController
{
	public:
		ros::NodeHandle node_handle;
		ros::Publisher velocity_publisher;
		ros::Subscriber pose_subscriber;
		LaserReader laser_reader;
		geometry_msgs::Pose pose;
		std::vector<Obstacle> obstacles;

		RRT rrt;
		ReadMapModule map;

		RobotController();
		void run();

	private:
		void pose_callback(const nav_msgs::Odometry::ConstPtr &o);
		void print_pose();
		void gather_obstacles();
		RRTNode *pick_node(int max_x, int max_y, int min_x, int min_y); 
};


#endif