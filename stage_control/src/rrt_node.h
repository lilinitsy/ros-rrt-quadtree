#include <vector>

#include <ros/ros.h>
#include <geometry_msgs/Pose.h>


struct RRTNode
{
    geometry_msgs::Pose pose;
    std::vector<RRTNode*> neighbours;
};