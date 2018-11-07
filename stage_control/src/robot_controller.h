#include <ros/ros.h>

#include <geometry_msgs/Point.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/String.h>

#include "laser_reader.h"
#include "rrt.h"

class RobotController
{
	public:
		ros::NodeHandle node_handle;
		ros::Publisher velocity_publisher;
		ros::Subscriber pose_subscriber;
		LaserReader laser_reader;
		geometry_msgs::Pose pose;
		//RRT rrt;

		RobotController();
		void run();

	private:
		void pose_callback(const nav_msgs::Odometry::ConstPtr &o);
		void print_pose();

		bool valid_point(geometry_msgs::Point p);
		//RRTNode *pick_node(); 

};