#ifndef RRT_H
#define RRT_H


#include <vector>


#include <ros/ros.h>
#include <geometry_msgs/Point.h>
#include <std_msgs/String.h>


#include "rrt_node.h"



class RRT
{
    public:
        std::vector<RRTNode*> nodes;
        geometry_msgs::Point  goal;
        bool goal_reached = false;

        RRT();
        void extend(geometry_msgs::Point point);
            // find closest node to tthis point
            // then have that node add a child towards that point
            // maybe do a backwards-hillclimbing approach for valid points?
            // or just have a wittle bit of randomness...
            // well anyway, after that, add those children to this nodes vector.

};


#endif