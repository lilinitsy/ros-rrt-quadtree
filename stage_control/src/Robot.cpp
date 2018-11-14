#include <iostream>
#include <sstream>

#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Point32.h>
#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/Twist.h>

#include <opencv2/imgproc.hpp>


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
	cv::Vec2d position = cv::Vec2d(-15.277f, 13.266f);
	//cv::Vec2i pixel_position = map.get_pixel_coordinates(-15.277f, 13.266f, 689, 809, 28.806f);
	cv::Vec2i pixel_position = cv::Vec2i(233, 203);
	cv::Vec2i goal_position = cv::Vec2i(494, 261);

	rrt = RRT(goal_position, 4); // step size 3 pixels
	rrt.build_rrt(pixel_position, map, 10);
	printf("RRT built\n");
	printf("RRT nodes size: %lu\n", rrt.nodes.size());
	printf("World position: %f %f\n", pose.position.x, pose.position.y);
	printf("pixel_position: %d %d\n", pixel_position.val[0], pixel_position.val[1]);
	write_image(689, 809);

	// initialize djikstraRRT path here
	// after doing that, go through the path and convert coordinates

	while(ros::ok())
	{
		ros::spinOnce();
	}
}

void Robot::write_image(int rows, int columns)
{
	//cv::Mat output = cv::Mat::zeros(rows, columns, CV_8UC3);
	cv::Mat image = cv::imread("src/ros-rrt-quadtree/bitmaps/autolab.png", cv::IMREAD_COLOR);
	
//Scalar intensity = img.at<uchar>(y, x);
	for(unsigned int i = 0; i < rrt.nodes.size(); i++)
	{
		for(int j = 0; j < rrt.nodes[i]->children.size(); j++)
		{
			cv::line(image, rrt.nodes[i]->position, rrt.nodes[i]->children[j]->position, cv::Scalar(255, 255, 0));
		}
	}

	cv::namedWindow("Graph", CV_WINDOW_AUTOSIZE);
	cv::imshow("Graph", image);
	cv::imwrite("src/ros-rrt-quadtree/rrt.png", image);
	cv::waitKey(0);
}