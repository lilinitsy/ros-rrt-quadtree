#ifndef RRT_NODE_H
#define RRT_NODE_H


#include <vector>

#include <ros/ros.h>
#include <geometry_msgs/Point.h>



class RRTNode
{
    public:
        geometry_msgs::Point position;
        std::vector<RRTNode*> children;

        RRTNode();
        RRTNode(geometry_msgs::Point p);
};


#endif