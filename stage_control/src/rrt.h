#include <vector>


#include <ros/ros.h>
#include <std_msgs/String.h>


#include "rrt_node.h"



class RRT
{
    std::vector<RRTNode*> nodes;
};