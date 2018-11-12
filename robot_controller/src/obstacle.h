#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <opencv2/core/core.hpp>
#include <ros/ros.h>



// TODO for final?
// General obstacle container
// Maybe some types just inherit so I can have a universal access container...
class Obstacle
{
    public:
		cv::Vec3f position;
        cv::Vec3f size;

        Obstacle();
        Obstacle(cv::Vec3f p, cv::Vec3f s);
};


#endif