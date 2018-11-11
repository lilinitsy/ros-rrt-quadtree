#ifndef RRT_H
#define RRT_H


#include <vector>


#include <ros/ros.h>
#include <geometry_msgs/Point.h>
#include <std_msgs/String.h>


#include "rrt_node.h"

#include "map_read.h"


class RRT
{
    public:
        std::vector<RRTNode*>nodes;
        geometry_msgs::Point goal;
        bool goal_reached = false;

        RRT();
        void extend(geometry_msgs::Point point);
        void add_node(RRTNode *node);
            // find closest node to tthis point
            // then have that node add a child towards that point
            // maybe do a backwards-hillclimbing approach for valid points?
            // or just have a wittle bit of randomness...
            // well anyway, after that, add those children to this nodes vector.
        bool valid_point(geometry_msgs::Point point, geometry_msgs::Point robot_position, ReadMapModule map);
        cv::Vec2i get_pixel_coordinates(float world_x, float world_y, float world_max_x, float world_max_y, int pixel_rows, int pixel_cols, float pixels_to_meters);

    private:
        unsigned int get_closest_node_to_point(geometry_msgs::Point point);
        float distance(geometry_msgs::Point a, geometry_msgs::Point b);
};


#endif