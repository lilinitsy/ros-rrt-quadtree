#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

#include "laser_reader.h"


LaserReader::LaserReader()
{
	// publish the laser scan
	// this will make an entry: topic: laser_scan 
	laser_scan_subscriber = node_handle.subscribe<sensor_msgs::LaserScan>("base_scan", 60, &LaserReader::laser_scan_callback, this);
	laser_scan_publisher = node_handle.advertise<sensor_msgs::LaserScan>("laser_scan", 60);
}	


void LaserReader::laser_scan_callback(const	sensor_msgs::LaserScan::ConstPtr &scan)
{
	int num_readings = (scan->angle_max - scan->angle_min) / scan->angle_increment	;

	std::stringstream scan_info;
	scan_info << std::endl;
	scan_info << "Num readings: " << num_readings << std::endl;
	scan_info << "closest distance: " << std::fixed << std::setprecision(2) << closest_collision(scan, num_readings) << std::endl;
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