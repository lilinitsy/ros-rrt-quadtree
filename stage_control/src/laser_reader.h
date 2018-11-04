#include <ros/ros.h>

#include <sensor_msgs/LaserScan.h>
#include <std_msgs/String.h>


// Try openCV for reading in the map
// so that the laser_scanner doesn't have to be used
// for this assgn. IT WILL need to be used for final.
class LaserReader
{
	public:
		ros::NodeHandle node_handle;
		ros::Publisher laser_scan_publisher;
		ros::Subscriber laser_scan_subscriber;
		sensor_msgs::LaserScan::ConstPtr laser_scan;
		
		LaserReader();
		void read_laser_scanner();
		void scan_world();

	private:
		void laser_scan_callback(const sensor_msgs::LaserScan::ConstPtr &scan);
		void sort_laser_scan_ranges(int num_readings);

		float closest_collision(int num_readings); // may be defunct

};