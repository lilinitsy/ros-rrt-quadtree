#include <vector>

#include <ros/ros.h>
#include <geometry_msgs/Pose.h>


struct Node
{
    geometry_msgs::Pose pose;
    std::vector<Node*> neighbours;
};