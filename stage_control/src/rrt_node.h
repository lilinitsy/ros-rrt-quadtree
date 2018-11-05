#include <vector>

#include <ros/ros.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Pose.h>



class RRTNode
{
    public:
        geometry_msgs::Pose pose;
        std::vector<RRTNode*> children;

        RRTNode();

};