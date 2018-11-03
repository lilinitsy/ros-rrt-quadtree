#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

#include "laser_reader.h"


/*
	Apparently, stage publishes laser scanner data to
		/base_scan

	SO laser_reader needs to SUBSCRIBE to /base_scan




~/ros_ws » rosmsg show sensor_msgs/LaserScan                lappytoppy@lappytop
std_msgs/Header header
  uint32 seq
  time stamp
  string frame_id
float32 angle_min
float32 angle_max
float32 angle_increment
float32 time_increment
float32 scan_time
float32 range_min
float32 range_max
float32[] ranges
float32[] intensities

*/

// Somewhat redundant file; demonstrates subscribing to laser scan topic data (published by stage) and republishing that



//sensor_msgs::LaserScan laser_scan;


LaserReader::LaserReader()
{
	// publish the laser scan
	// this will make an entry: topic: laser_scan // publisher: laser_reader (.cpp)
	laser_scan_subscriber = node_handle.subscribe<sensor_msgs::LaserScan>("base_scan", 60, &LaserReader::laser_scan_callback, this);
	laser_scan_publisher = node_handle.advertise<sensor_msgs::LaserScan>("laser_scan", 60);
}	


void LaserReader::laser_scan_callback(const	sensor_msgs::LaserScan::ConstPtr &scan)
{
	double angle_min = scan->angle_min;
	double angle_max = scan->angle_max;
	double angle_increment = scan->angle_increment;
	int num_readings = (scan->angle_max - scan->angle_min) / scan->angle_increment	;

	//laser_scan.header = scan->header;
	//laser_scan.angle_min = scan->angle_min;

	std::stringstream scan_info;
	scan_info << std::endl;
	scan_info << "Num readings: " << num_readings << std::endl;
	scan_info << "closest distance: " << std::fixed << std::setprecision(2) << closest_collision(scan, num_readings) << std::endl;
//	std::string scaninfo = 
	ROS_INFO_STREAM("SCAN INFO: " << scan_info.str());
}



void LaserReader::scan_world()
{
	ros::Rate rate(10);
	ROS_INFO("Scan world");
	while(ros::ok())
	{
		// to get ros::ok() == true
		ros::spinOnce();

	}
}


float LaserReader::closest_collision(const sensor_msgs::LaserScan::ConstPtr &scan, int num_readings)
{
	std::vector<float> ranges = scan->ranges;
	std::sort(ranges.begin(), ranges.end());
	return ranges[0];
}