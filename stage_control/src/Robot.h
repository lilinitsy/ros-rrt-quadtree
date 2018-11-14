#ifndef ROBOT_H
#define ROBOT_H


#include <vector>

#include <ros/ros.h>

#include <geometry_msgs/Point.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/String.h>


#include "DjikstraRRT.h"
#include "Map.h"
#include "Quadtree.h"
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
		QuadTree *quadtree;
		ReadMapModule map;
		DjikstraRRT rrt_path;

		Robot();
		void run_rrt();
		void run_quadtree();

	private:
		void pose_callback(const nav_msgs::Odometry::ConstPtr &o);
		void print_pose();
		void rrt_write_image(int rows, int columns);
		void quadtree_write_image(int rows, int columns, std::vector<QuadTree*> clear_boxes);
};



#endif