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


void Robot::run_rrt(int start_x, int start_y, int end_x, int end_y)
{
	ros::Rate rate(1);
	cv::Vec2d position = cv::Vec2d(-15.277f, 13.266f);
	//cv::Vec2i pixel_position = map.get_pixel_coordinates(-15.277f, 13.266f, 689, 809, 28.806f);
	cv::Vec2i pixel_position = cv::Vec2i(start_x, start_y);
	cv::Vec2i goal_position = cv::Vec2i(end_x, end_y);

	rrt = RRT(goal_position, 4); // step size 3 pixels
	rrt.build_rrt(pixel_position, map, 10);
	printf("RRT built\n");
	printf("RRT nodes size: %lu\n", rrt.nodes.size());
	printf("World position: %f %f\n", pose.position.x, pose.position.y);
	printf("pixel_position: %d %d\n", pixel_position.val[0], pixel_position.val[1]);
	
	rrt_path.find_path(rrt.nodes, rrt.nodes[0], goal_position);
	printf("Djikstra RRT path found\n");
	printf("Size of djikstra RRT path: %lu\n", rrt_path.path.size());
	
	rrt_write_image(689, 809);

	// initialize djikstraRRT path here
	// after doing that, go through the path and convert coordinates

	while(ros::ok())
	{
		ros::spinOnce();
	}
}


void Robot::run_quadtree(int start_x, int start_y, int end_x, int end_y)
{
	ros::Rate rate(1);
	cv::Vec2d position = cv::Vec2d(-15.277f, 13.266f);
	//cv::Vec2i pixel_position = map.get_pixel_coordinates(-15.277f, 13.266f, 689, 809, 28.806f);
	cv::Vec2i pixel_position = cv::Vec2i(233, 203);
	cv::Vec2i goal_position = cv::Vec2i(494, 261);
	std::vector<QuadTree*> clear_boxes;
	AABB boundary = AABB(cv::Vec2i(404, 344), 400, 340);
	quadtree = new QuadTree(boundary, 10);

	quadtree->build_quadtree(map, clear_boxes);
	printf("clear_boxes size: %lu\n", clear_boxes.size());

	quadtree_write_image(689, 809, clear_boxes);

	while(ros::ok())
	{
		ros::spinOnce();
	}
}


void  Robot::quadtree_write_image(int rows, int columns, std::vector<QuadTree*> clear_boxes)
{
	cv::Mat image = cv::imread("src/ros-rrt-quadtree/bitmaps/autolab.png", cv::IMREAD_COLOR);

	for(unsigned int i = 0; i < clear_boxes.size(); i++)
	{
		AABB boundary = clear_boxes[i]->boundary;
		// POINTS:
		int bottom_left_x = boundary.center.val[0] - boundary.half_dimension_x;
		int bottom_left_y = boundary.center.val[1] - boundary.half_dimension_y;
		cv::Vec2i bottom_left = cv::Vec2i(bottom_left_x, bottom_left_y);

		int top_left_x = boundary.center.val[0] - boundary.half_dimension_x;
		int top_left_y = boundary.center.val[1] + boundary.half_dimension_y;
		cv::Vec2i top_left = cv::Vec2i(top_left_x, top_left_y);

		int bottom_right_x = boundary.center.val[0] + boundary.half_dimension_x;
		int bottom_right_y = boundary.center.val[1] - boundary.half_dimension_y;
		cv::Vec2i bottom_right = cv::Vec2i(bottom_right_x, bottom_right_y);

		int top_right_x = boundary.center.val[0] + boundary.half_dimension_x;
		int top_right_y = boundary.center.val[1] + boundary.half_dimension_y;
		cv::Vec2i top_right = cv::Vec2i(top_right_x, top_right_y);

		cv::line(image, bottom_left, top_left, cv::Scalar(0, 255, 0));
		cv::line(image, bottom_left, bottom_right, cv::Scalar(0, 255, 0));
		cv::line(image, top_left, top_right, cv::Scalar(0, 255, 0));
		cv::line(image, top_right, bottom_right, cv::Scalar(0, 255, 0));
	}

	cv::namedWindow("Graph", CV_WINDOW_AUTOSIZE);
	cv::imshow("Graph", image);
	cv::imwrite("src/ros-rrt-quadtree/quadtree.png", image);
	cv::waitKey(0);

}


void Robot::rrt_write_image(int rows, int columns)
{
	//cv::Mat output = cv::Mat::zeros(rows, columns, CV_8UC3);
	cv::Mat image = cv::imread("src/ros-rrt-quadtree/bitmaps/autolab.png", cv::IMREAD_COLOR);
	
//Scalar intensity = img.at<uchar>(y, x);
	// draw the tree in 255b, 255g... since it's BGR?
	for(unsigned int i = 0; i < rrt.nodes.size(); i++)
	{
		for(unsigned int j = 0; j < rrt.nodes[i]->children.size(); j++)
		{
			cv::line(image, rrt.nodes[i]->position, rrt.nodes[i]->children[j]->position, cv::Scalar(255, 255, 0));
		}
	}

	cv::circle(image, rrt.nodes[0]->position, 10, cv::Scalar(255, 0, 255));
	cv::circle(image, rrt.nodes[rrt.nodes.size() - 1]->position, 10, cv::Scalar(0, 255, 0));

	/*
	// draw the path in.... RED???
	for(unsigned int i = 1; i < rrt_path.path.size() - 1; i++)
	{
		cv::line(image, rrt_path.path[i].previous->position, rrt_path.path[i].node->position, cv::Scalar(0, 0, 255));
	}
	*/


	cv::namedWindow("Graph", CV_WINDOW_AUTOSIZE);
	cv::imshow("Graph", image);
	cv::imwrite("src/ros-rrt-quadtree/rrt.png", image);
	cv::waitKey(0);
}