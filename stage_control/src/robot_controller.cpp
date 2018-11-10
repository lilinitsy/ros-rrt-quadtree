#include <iostream>
#include <sstream>

#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Point32.h>
#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/Twist.h>


#include "robot_controller.h"


/*
try subscribing to /map?
*/


RobotController::RobotController()
{
	velocity_publisher = node_handle.advertise<geometry_msgs::Twist>("cmd_vel", 1); // TODO: Look into this more
	pose_subscriber = node_handle.subscribe<nav_msgs::Odometry>("odom", 60, &RobotController::pose_callback, this);
	laser_reader = LaserReader();
	gather_obstacles();
	map = ReadMapModule("src/ros-rrt-quadtree/bitmaps/autolab.png", 54.0f, 58.7f, 28.806f, obstacles); // will need to change at some point to not be hardcoded... or parse the willow world file.
}


void RobotController::pose_callback(const nav_msgs::Odometry::ConstPtr &o)
{
	pose.orientation = o->pose.pose.orientation;
	pose.position = o->pose.pose.position;
}


RRTNode *RobotController::pick_node(const int max_x, const int max_y, const int min_x, const int min_y)
{
//	printf("Max pixels x: %d\n", map.max_pixels.val[0]);
//	printf("Max pixels y: %d\n", map.max_pixels.val[1]);
	std::cout << "Max worldspace x: " << max_x << std::endl;
	std::cout << "Max space at y: " << max_y << std::endl;
	float x = min_x + static_cast<float>(rand()) /
		static_cast<float>(RAND_MAX /
			(max_x - min_x));
	
	
	float y = min_y + static_cast<float>(rand()) / 
		static_cast<float>(RAND_MAX / 
			(max_y - min_y));

	float z = 1;

	std::cout << "(x, y, z): (" << x << ", " << y << ", " << z << ")" << std::endl;
}


bool RobotController::valid_point(geometry_msgs::Point p)
{

}


// TODO: Think of a cool way to get all obstacles
void RobotController::gather_obstacles()
{
	// for now, hardcode
	obstacles.push_back(Obstacle(cv::Vec3f(-13.924, 25.020, 0), cv::Vec3f(0.5f, 0.5f, 0.5f)));	
}


void RobotController::print_pose()
{
	ROS_INFO_STREAM("Pose orientation: " << pose.orientation);
	ROS_INFO_STREAM("Pose position: " << pose.position);
}

void RobotController::run()
{
	// for some reason these have to be const or there's some memory corruption.
	const int max_x_pixels = map.max_pixels.val[0];
	const int max_y_pixels = map.max_pixels.val[1]; 

	//const int max_x = map.map[max_x_pixels - 1][max_y_pixels - 1].world_space.val[0];
	//const int max_y = map.map[max_x_pixels - 1][max_y_pixels - 1].world_space.val[1];
	// I have to set it cause ROS seems to be full of memory leaks? Fuck, idk
	// should probably do ROS with python in the future, but rn I'm too far in
	// I also don't remember how to write python
	const int max_x = 29;
	const int max_y = 28;
	const int min_x = -29;
	const int min_y = -28;

	ros::Rate rate(1);
	while(ros::ok())
	{
		ros::spinOnce();
		print_pose();
	//	while(!rrt.goal_reached)
	//	{
			pick_node(max_x, max_y, min_x, min_y);
	//	}

		// laser_reader.scan_world(); need a cmdline flag for this
	}
}